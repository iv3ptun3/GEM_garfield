#include <cstdlib>
#include <iostream>
#include <fstream>

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

#include "FieldMapBuilder.hpp"
#include "DataStruct.hpp"
#include "ParManager.hpp"
using namespace Garfield;

constexpr unsigned int BUFF_AVALANCHE = 1000000;

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        std::cout << "Usage : tgem [parameter file] [outfile] [nEvent]" << std::endl;
        return 0;
    }
    bool driftIons = false;
    bool magneticFieldOn = true;

    TFile* file = TFile::Open(argv[2], "RECREATE");
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
    
    ComponentElmer *fm1 = fmBuilder->buildGemFieldMap();
    ComponentConstant *fm2 = fmBuilder->buildMagneticField();

    // Create the sensor.
    Sensor sensor;
    sensor.AddComponent(fm1);
    sensor.AddComponent(fm2);
    sensor.SetArea(-tpcX/2, -tpcY/2, 0, tpcX/2, tpcY/2, dZp + dZ12 + dZ23 + dZu);

    AvalancheMicroscopic aval;
    aval.SetSensor(&sensor);
    aval.EnableMagneticField(magneticFieldOn);

    AvalancheMC drift;
    drift.SetSensor(&sensor);
    drift.SetDistanceSteps(2.e-4);
    drift.EnableMagneticField(true);
    TTree *tree = new TTree("tc", "tc");

    DriftEndPointsPtr endPoints;
    unsigned int evtId = 0;
    unsigned int np;
    int ne, ni;
    double xe1, ye1, ze1, te1, e1;
    double xe2, ye2, ze2, te2, e2;
    double xi1, yi1, zi1, ti1;
    double xi2, yi2, zi2, ti2;
    int status;
    unsigned int dataPtr;
    allocateDriftEndPointsPtr(&endPoints, BUFF_AVALANCHE);
    // event
    tree->Branch("evtId", &evtId, "evtId/i");
    tree->Branch("ne", &dataPtr, "ne/i");
    tree->Branch("xe1", endPoints.xe1, "xe1[ne]/D");
    tree->Branch("ye1", endPoints.ye1, "ye1[ne]/D");
    tree->Branch("ze1", endPoints.ze1, "ze1[ne]/D");
    tree->Branch("te1", endPoints.te1, "te1[ne]/D");
    tree->Branch("e1", endPoints.e1, "e1[ne]/D");
    tree->Branch("xe2", endPoints.xe2, "xe2[ne]/D");
    tree->Branch("ye2", endPoints.ye2, "ye2[ne]/D");
    tree->Branch("ze2", endPoints.ze2, "ze2[ne]/D");
    tree->Branch("te2", endPoints.te2, "te2[ne]/D");
    tree->Branch("e2", endPoints.e2, "e2[ne]/D");
    tree->Branch("status", endPoints.status, "status[ne]/i");
    // event loop
    for (evtId = 0; evtId < nEvt; ++evtId)
    {
        dataPtr = 0;
        aval.AvalancheElectron(0., 0., dZp + dZ12 + dZ23 + dZu * 0.999,
            0., 0.1, 0., 0., 0.);
        aval.GetAvalancheSize(ne, ni);
        np = aval.GetNumberOfElectronEndpoints();
        for (unsigned int j = 0; j < np; ++j)
        {
            aval.GetElectronEndpoint(j, endPoints.xe1[dataPtr], endPoints.ye1[dataPtr], endPoints.ze1[dataPtr], endPoints.te1[dataPtr], endPoints.e1[dataPtr],
                                     endPoints.xe2[dataPtr], endPoints.ye2[dataPtr], endPoints.ze2[dataPtr], endPoints.te2[dataPtr], endPoints.e2[dataPtr], endPoints.status[dataPtr]);
            if (driftIons)
            {
                drift.DriftIon(endPoints.xe1[dataPtr], endPoints.ye1[dataPtr], endPoints.ze1[dataPtr], endPoints.e1[dataPtr]);
                drift.GetIonEndpoint(0, xi1, yi1, zi1, ti1,
                                     xi2, yi2, zi2, ti2, status);
            }
            ++dataPtr;
        }
        std::cout << evtId << "/" << nEvt << " : " << dataPtr << "\n";
        tree->Fill();
        if((evtId + 1) % 1 == 0)
            tree->AutoSave("SaveSelf");

    }
    tree->Write();
    file->Close();
    std::cout << "File " << file->GetName() << " has been successfully saved." << std::endl;

    // delete allocated memory
    delete parMan;
    delete fm1;
    delete fm2;
    freeDriftEndPointsPtr(&endPoints);
    return 0;
}
