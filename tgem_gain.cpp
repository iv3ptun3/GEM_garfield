#include <cstdlib>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <time.h>

#include "TFile.h"
#include "TTree.h"

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

#include "DataStruct.hpp"
#include "ParManager.hpp"
using namespace Garfield;

std::unique_ptr<MediumMagboltz> initGasMixture(const ParManager *parMan);

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        std::cout << "Usage : tgem [parameter file] [outfile] [nEvent]" << std::endl;
        return 0;
    }
    TFile* file = TFile::Open(argv[2], "RECREATE");
    const unsigned int nEvt = atoi(argv[3]);

    // Initializing a global manager for configure parameters.
    auto parMan = ParManager::getInstance();
    parMan->initPars(argv[1]);     
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

    std::unique_ptr<MediumMagboltz> gas = initGasMixture(parMan);
    std::unique_ptr<ComponentElmer> componentGem = ComponentFactory::createElmer(gas.get(), fmFileName);

    // Create the sensor.
    Sensor sensor;
    sensor.AddComponent(componentGem.get());
    sensor.SetArea(-tpcX/2, -tpcY/2, 0, tpcX/2, tpcY/2, dZp + dZ12 + dZ23 + dZu);

    AvalancheMicroscopic aval;
    aval.SetSensor(&sensor);

    AvalancheMC drift;
    drift.SetSensor(&sensor);
    drift.SetDistanceSteps(2.e-4);
    TTree *tree = new TTree("tc", "tc");

    unsigned int evtId = 0;
    unsigned int np;
    int ne, ni;
    // event
    tree->Branch("evtId", &evtId, "evtId/i");
    tree->Branch("ne", &ne, "ne/i");
    // event loop
    const int timeOut = 216000; // 2.5 days
    for (evtId = 0; evtId < nEvt; ++evtId)
    {
        if((int)clock()/CLOCKS_PER_SEC > timeOut)
        {
            std::cout << "Time out, escaping for loop..." << std::endl;
            break;
        }
        aval.AvalancheElectron(0., 0., dZp + dZ12 + dZ23 + dZu * 0.999,
            0., 0.1, 0., 0., 0.);
        aval.GetAvalancheSize(ne, ni);
        std::cout << std::setw(8) << evtId << "/" << std::setw(8) <<  nEvt << " : " << std::setw(8) << ne << "\n";
        tree->Fill();
        if((evtId + 1) % 1 == 0)
            tree->AutoSave("SaveSelf");
    }
    tree->Write();
    file->Close();
    std::cout << "File " << file->GetName() << " has been successfully saved." << std::endl;

    // delete allocated memory
    delete parMan;
    return 0;
}

std::unique_ptr<MediumMagboltz> initGasMixture(const ParManager *parMan)
{
    auto gas = MediumMagboltzFactory::createGasMixture(
        parMan->getParS("GAS_MEDIUM"), parMan->getParD("GAS_MEDIUM_FRAC"),
        parMan->getParS("GAS_QUENCHING"), parMan->getParD("GAS_QUENCHING_FRAC"));
    gas->SetPressure(parMan->getParD("GAS_PRESSURE"));
    gas->EnablePenningTransfer(parMan->getParD("GAS_PENNING_EFF"), parMan->getParD("GAS_PENNING_LAMB"), parMan->getParS("GAS_MEDIUM"));
    gas->Initialise(true);
    return std::move(gas);
}