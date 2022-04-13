#include <cstdlib>
#include <iostream>
#include <fstream>

#include "TFile.h"
#include "TTree.h"
#include "TApplication.h"

#include "Garfield/ComponentElmer.hh"
#include "Garfield/ViewField.hh"
#include "Garfield/ViewFEMesh.hh"
#include "Garfield/MediumMagboltz.hh"
#include "Garfield/Sensor.hh"
#include "Garfield/AvalancheMicroscopic.hh"
#include "Garfield/AvalancheMC.hh"
#include "Garfield/Random.hh"

#include "ComponentFactory.hpp"
#include "MediumMagboltzFactory.hpp"

#include "ParManager.hpp"
#include "SrimTrackBuilder.hpp"
using namespace Garfield;

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        std::cout << "Usage : simple_track [parameter file] [outfile] [nEvent]" << std::endl;
        return 0;
    }
    bool driftIons = false;
    bool magneticFieldOn = true;

    // TFile* file = TFile::Open(argv[2], "RECREATE");
    const unsigned int nEvt = atoi(argv[3]);

    // Initializing a global manager for configure parameters.
    auto parMan = ParManager::getInstance();
    parMan->initPars(argv[1]); 
    parMan->listPars();
    // tripple gem dimension
    const double pitch = parMan->getParD("PITCH");
    const double tD = parMan->getParD("T_DIEL");
    const double dZp = parMan->getParD("DZ_PADPLANE");
    const double dZ12 = parMan->getParD("DZ_GEM12");
    const double dZ23 = parMan->getParD("DZ_GEM23");
    const double dZe = parMan->getParD("DZ_ELECTRODE");
    const double dZu = parMan->getParD("DZ_UPPERPLANE");
    const double tpcX = parMan->getParD("TPC_X");
    const double tpcY = parMan->getParD("TPC_Y");
    // voltage
    const double dvg1 = parMan->getParD("DV_GEM1");
    const double dvg2 = parMan->getParD("DV_GEM2");
    const double dvg3 = parMan->getParD("DV_GEM3");
    // electric field density
    const double eTrans = parMan->getParD("E_TRANS");
    const double eDrift = parMan->getParD("E_DRIFT");
    const double eInduction = parMan->getParD("E_INDUCTION");
    // magnetic field
    const double bField = parMan->getParD("B_Z");
    // elmer data name
    const std::string fmFileName = parMan->getParS("SCRIPT_NAME");
    const string scriptDir = parMan->getParS("SCRIPT_NAME");
    const string gasFileName = parMan->getParS("GAS_FILE");
    const string srimFileName = parMan->getParS("SRIM_FILE");

    unique_ptr<MediumMagboltz> gas = MediumMagboltzFactory::createFromGasFile(gasFileName);

    std::unique_ptr<ComponentConstant> componentDrift = ComponentFactory::createConstE(
        gas.get(), eDrift,
        -tpcX / 2, -tpcY / 2, dZp + dZ12 + dZ23 + dZu,
        tpcX, tpcY, dZp + dZ12 + dZ23 + dZu + dZe);
    std::unique_ptr<ComponentConstant> componentMag = ComponentFactory::createConstB(
        gas.get(), bField,
        -tpcX / 2, -tpcY / 2, 0.,
        tpcX, tpcY, dZp + dZ12 + dZ23 + dZu + dZe);

    Sensor sensor;
    sensor.AddComponent(componentDrift.get());
    sensor.AddComponent(componentMag.get());
    sensor.SetArea(-tpcX / 2, -tpcY / 2, dZp + dZ12 + dZ23 + dZu, tpcX / 2, tpcY / 2, dZp + dZ12 + dZ23 + dZu + dZe);
    
    AvalancheMicroscopic aval;
    
    aval.SetSensor(&sensor);
    aval.EnableMagneticField(magneticFieldOn);
    
    AvalancheMC drift;
    drift.SetSensor(&sensor);
    drift.SetDistanceSteps(2.e-4);


    string gas1 = parMan->getParS("GAS1"), gas2 = parMan->getParS("GAS2");
    int frac1 = parMan->getParI("FRAC1"), frac2 = parMan->getParI("FRAC2");
    // build TrackSrim instance
    SrimTrackBuilder stBuilder;
    stBuilder.setSrimFileName(srimFileName);
    stBuilder.setPressure(parMan->getParI("PRESSURE"));
    stBuilder.setComposition(gas1, frac1, gas1, frac2);
    auto track = stBuilder.build(&sensor);
    track->SetKineticEnergy(5.e6);
    track->SetTargetClusterSize(100);

    ViewDrift driftView;
    aval.EnablePlotting(&driftView);
    drift.EnablePlotting(&driftView);
    track->EnablePlotting(&driftView);
    for(int evt = 0; evt < nEvt;++evt)
    {
        track->NewTrack(0, 0, dZp + dZ12 + dZ23 + dZu + dZe / 2, 0, 1, 0, 0);
        double xc = 0., yc = 0., zc = 0., tc = 0., ec = 0., extra = 0.;
        int nc = 0;
        int ncTotal = 0;
        double ecTotal = 0;
        while (track->GetCluster(xc, yc, zc, tc, nc, ec, extra))
        {
            ncTotal += nc;
            ecTotal += ec;
            std::cout << nc << " ionized electrons in a cluster at (" << xc << " cm, " << yc << " cm, " << zc << " cm) : " << ec << " eV\n";
            for (int k = 0; k < nc; ++k)
            {
                // aval.DriftElectron(xc, yc, zc, tc, ec/nc);
            }
        }
        std::cout << "Total # of ionized electrons : " << ncTotal << std::endl;
        std::cout << "Total deposited energy       : " << ecTotal << " eV" << std::endl;
    }
    TApplication app("app", &argc, argv);    
    TCanvas* c2 = new TCanvas();
    c2->SetRightMargin(0.15);
    track->PlotEnergyLoss();
    track->PlotRange();
    TCanvas* c1 = new TCanvas();
    driftView.SetCanvas(c1);
    driftView.Plot();
    app.Run(true);
    return 0;
}