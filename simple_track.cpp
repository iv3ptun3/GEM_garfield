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

#include "FieldMapBuilder.hpp"
#include "ParManager.hpp"
#include "SrimTrackBuilder.hpp"
using namespace Garfield;

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        std::cout << "Usage : tgem_track [parameter file] [outfile] [nEvent]" << std::endl;
        return 0;
    }
    bool driftIons = false;
    bool magneticFieldOn = true;

    // TFile* file = TFile::Open(argv[2], "RECREATE");
    const unsigned int nEvt = atoi(argv[3]);

    // Initializing global managers.
    auto parMan = ParManager::getInstance();
    parMan->initPars(argv[1]);
    // parMan->listPars();

    // tripple gem dimension
    const double pitch = parMan->getParD("PITCH");
    const double tD = parMan->getParD("T_DIEL");
    const double dZp = parMan->getParD("DZ_PADPLANE");
    const double dZ12 = parMan->getParD("DZ_GEM12");
    const double dZ23 = parMan->getParD("DZ_GEM23");
    const double dZu = parMan->getParD("DZ_UPPERPLANE");
    const double dZe = parMan->getParD("DZ_ELECTRODE");
    const double tpcX = parMan->getParD("TPC_X");
    const double tpcY = parMan->getParD("TPC_Y");

    // get a field map from the builder
    FieldMapBuilder *fmBuilder = new FieldMapBuilder();
    fmBuilder->initGas();
    // ComponentElmer *fm1 = fmBuilder->buildGemFieldMap();
    ComponentConstant *fm2 = fmBuilder->buildDriftFieldMap();
    ComponentConstant *fm3 = fmBuilder->buildMagneticField();

    // Create the sensor.
    Sensor sensor;
    // sensor.AddComponent(fm1);
    sensor.AddComponent(fm2);
    sensor.AddComponent(fm3);
    sensor.SetArea(-tpcX / 2, -tpcY / 2, 0, tpcX / 2, tpcY / 2, dZp + dZ12 + dZ23 + dZu + dZe);
    
    AvalancheMicroscopic aval;
    aval.SetSensor(&sensor);
    aval.EnableMagneticField(magneticFieldOn);
    
    AvalancheMC drift;
    drift.SetSensor(&sensor);
    drift.SetDistanceSteps(2.e-4);

    // build TrackSrim instance
    SrimTrackBuilder stBuilder(parMan->getParS("SRIM_DATA"));
    stBuilder.addCompound(parMan->getParS("GAS_MEDIUM"), parMan->getParD("GAS_MEDIUM_FRAC"),
                          parMan->getParD("GAS_MEDIUM_A"), parMan->getParD("GAS_MEDIUM_Z"), 
                          parMan->getParD("GAS_MEDIUM_FANO"), parMan->getParD("GAS_MEDIUM_WORK"));
    stBuilder.addCompound(parMan->getParS("GAS_QUENCHING"), parMan->getParD("GAS_QUENCHING_FRAC"),
                          parMan->getParD("GAS_QUENCHING_A"), parMan->getParD("GAS_QUENCHING_Z"), 
                          parMan->getParD("GAS_QUENCHING_FANO"), parMan->getParD("GAS_QUENCHING_WORK"));
    stBuilder.print();
    TrackSrim *track = stBuilder.build(&sensor);
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
