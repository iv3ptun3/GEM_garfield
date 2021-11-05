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

    // get a field map from the builder
    FieldMapBuilder *fmBuilder = new FieldMapBuilder();
    fmBuilder->initGas();
    ComponentElmer *fm = fmBuilder->buildGemFieldMap();

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
    // voltage
    const double dvg1 = parMan->getParD("DV_GEM1");
    const double dvg2 = parMan->getParD("DV_GEM2");
    const double dvg3 = parMan->getParD("DV_GEM3");
    // electric field density
    const double eTrans = parMan->getParD("E_TRANS");
    const double eDrift = parMan->getParD("E_DRIFT");
    const double eInduction = parMan->getParD("E_INDUCTION");

    // Create the sensor.
    Sensor sensor;
    sensor.AddComponent(fm);
    sensor.SetArea(-tpcX/2, -tpcY/2, 0, tpcX/2, tpcY/2, dZp + dZ12 + dZ23 + dZu);

    AvalancheMicroscopic aval;
    aval.SetSensor(&sensor);
    aval.EnableMagneticField(magneticFieldOn);

    AvalancheMC drift;
    drift.SetSensor(&sensor);
    drift.SetDistanceSteps(2.e-4);

    TrackClustersPtr clusters;
    allocateTrackClustersPtr(&clusters, nEvt);
    clusters.evtId = 0;
    clusters.trkId = 0;
    clusters.nc = nEvt;
    for (int i = 0; i < nEvt; ++i)
    {
        clusters.xc[i] = 0.0;
        clusters.yc[i] = 0.0;
        clusters.zc[i] = dZp + dZ12 + dZ23 + dZu * 0.999;
        clusters.tc[i] = 0.0;
        clusters.ec[i] = 0.0;
    }
    TTree *tree = new TTree("tc", "tc");

    DriftEndPointsPtr endPoints;
    unsigned int dataPtr;
    unsigned int np;
    int ne, ni;
    double xe1, ye1, ze1, te1, e1;
    double xe2, ye2, ze2, te2, e2;
    double xi1, yi1, zi1, ti1;
    double xi2, yi2, zi2, ti2;
    int status;
    allocateDriftEndPointsPtr(&endPoints, BUFF_AVALANCHE);
    // event
    tree->Branch("evtId", &clusters.evtId, "evtId/i");
    // tracks in an event
    tree->Branch("trkId", &clusters.trkId, "trkId/i");
    // clusters on trajectory
    tree->Branch("cltId", &endPoints.cltId, "cltId/i");
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
    for (endPoints.cltId = 0; endPoints.cltId < clusters.nc; ++endPoints.cltId)
    {
        dataPtr = 0;
        aval.AvalancheElectron(clusters.xc[endPoints.cltId], clusters.yc[endPoints.cltId], clusters.zc[endPoints.cltId],
            clusters.ec[endPoints.cltId], clusters.tc[endPoints.cltId],
            0., 0., 0.);
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
        std::cout << endPoints.cltId << "/" << nEvt << " : " << dataPtr << "\n";
        tree->Fill();
    }
    tree->Write();
    file->Close();
    std::cout << "File " << file->GetName() << " has been successfully saved." << std::endl;

    // delete allocated memory
    delete parMan;
    delete fm;
    freeTrackClustersPtr(&clusters);
    freeDriftEndPointsPtr(&endPoints);
    return 0;    
    /*
    TCanvas *cf = new TCanvas("cf", "", 600, 600);
    cf->SetLeftMargin(0.16);
    
    ViewDrift driftView;

    constexpr bool plotDrift = true;
    if (plotDrift)
    {
        aval.EnablePlotting(&driftView);
        drift.EnablePlotting(&driftView);
    }

    constexpr unsigned int nEvents = 10;
    for (unsigned int i = 0; i < nEvents; ++i)
    {
        std::cout << i << "/" << nEvents << "\n";
        // Randomize the initial position.
        const double x0 = -0.5 * pitch + RndmUniform() * pitch;
        const double y0 = -0.5 * pitch + RndmUniform() * pitch;
        const double z0 = 0.02;
        const double t0 = 0.;
        const double e0 = 0.1;
        aval.AvalancheElectron(x0, y0, z0, t0, e0, 0., 0., 0.);
        int ne = 0, ni = 0;
        aval.GetAvalancheSize(ne, ni);
        const unsigned int np = aval.GetNumberOfElectronEndpoints();
        double xe1, ye1, ze1, te1, e1;
        double xe2, ye2, ze2, te2, e2;
        double xi1, yi1, zi1, ti1;
        double xi2, yi2, zi2, ti2;
        int status;
        for (unsigned int j = 0; j < np; ++j)
        {
            aval.GetElectronEndpoint(j, xe1, ye1, ze1, te1, e1,
                                     xe2, ye2, ze2, te2, e2, status);
            drift.DriftIon(xe1, ye1, ze1, te1);
            drift.GetIonEndpoint(0, xi1, yi1, zi1, ti1,
                                 xi2, yi2, zi2, ti2, status);
        }
    }
    if (plotDrift)
    {
        TCanvas *cd = new TCanvas();
        constexpr bool plotMesh = true;
        if (plotMesh)
        {
            ViewFEMesh *meshView = new ViewFEMesh();
            meshView->SetArea(-2 * pitch, -2 * pitch, -0.02,
                              2 * pitch, 2 * pitch, 0.02);
            meshView->SetCanvas(cd);
            meshView->SetComponent(&fm);
            // x-z projection.
            meshView->SetPlane(0, -1, 0, 0, 0, 0);
            meshView->SetFillMesh(true);
            meshView->SetColor(0, kGray);
            // Set the color of the kapton.
            meshView->SetColor(2, kYellow + 3);
            meshView->EnableAxes();
            meshView->SetViewDrift(&driftView);
            meshView->Plot();
        }
        else
        {
            driftView.SetPlane(0, -1, 0, 0, 0, 0);
            driftView.SetArea(-2 * pitch, -0.02, 2 * pitch, 0.02);
            driftView.SetCanvas(cd);
            constexpr bool twod = true;
            driftView.Plot(twod);
        }
    }*/
}
