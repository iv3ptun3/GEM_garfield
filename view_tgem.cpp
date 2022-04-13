#include <cstdlib>
#include <iostream>
#include <fstream>

#include <TApplication.h>
#include <TCanvas.h>
#include <TH1F.h>

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

using namespace Garfield;

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cout << "Usage : view_tgem [parameter file]" << std::endl;
        return 0;
    }
    // Initializing a global manager for configure parameters.
    auto parMan = ParManager::getInstance();
    parMan->initPars(argv[1]);    
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
    // magnetic field
    const double bField = parMan->getParD("B_Z");
    // elmer data name
    const std::string fmFileName = parMan->getParS("SCRIPT_NAME");
    const std::string gasFileName = parMan->getParS("GAS_FILE");

    std::unique_ptr<MediumMagboltz> gas = MediumMagboltzFactory::createFromGasFile(gasFileName);
    std::unique_ptr<ComponentElmer> componentGem = ComponentFactory::createElmer(gas.get(), fmFileName);

    TApplication app("app", &argc, argv);

    // plot equipotential lines of GEM1
    TCanvas *cf1 = new TCanvas("cf1", "Potential plot of GEM1", 600, 600); cf1->SetLeftMargin(0.16);
    TCanvas *cf2 = new TCanvas("cf2", "Potential plot of GEM2", 600, 600); cf2->SetLeftMargin(0.16);
    TCanvas *cf3 = new TCanvas("cf3", "Potential plot of GEM3", 600, 600); cf3->SetLeftMargin(0.16);
    TCanvas *cf4 = new TCanvas("cf4", "Potential plot", 600, 600); cf4->SetLeftMargin(0.16);

    ViewField vf;
    vf.SetComponent(componentGem.get());
    vf.SetNumberOfContours(50);
    vf.SetPlaneXZ();

    vf.SetCanvas(cf1);
    vf.SetVoltageRange(
        -dZp * eInduction - dvg1 - dZ12 * eTrans - dvg2 - dZ23 * eTrans - 1.5 * dvg3,
        -dZp * eInduction - dvg1 - dZ12 * eTrans - dvg2 - dZ23 * eTrans + 1.5 * dvg3);
    vf.SetArea(-1.5 * pitch, dZp + dZ12 + dZ23 - 3 * tD, 1.5 * pitch, dZp + dZ12 + dZ23 + 3 * tD);
    vf.PlotContour();

    vf.SetCanvas(cf2);
    vf.SetVoltageRange(
        -dZp * eInduction - dvg1 - dZ12 * eTrans - 1.5 * dvg2,
        -dZp * eInduction - dvg1 - dZ12 * eTrans + 1.5 * dvg2);
    vf.SetArea(-1.5 * pitch, dZp + dZ12 - 3 * tD, 1.5 * pitch, dZp + dZ12 + 3 * tD);
    vf.PlotContour();

    vf.SetCanvas(cf3);
    vf.SetVoltageRange(
        -dZp * eInduction - 1.5 * dvg1,
        -dZp * eInduction + 1.5 * dvg1);
    vf.SetArea(-1.5 * pitch, dZp - 3 * tD, 1.5 * pitch, dZp + 3 * tD);
    vf.PlotContour();

    vf.SetCanvas(cf4);
    vf.SetVoltageRange(-dZp * eInduction - dvg3 - dZ23 * eTrans - dvg2 - dZ12 * eTrans - dvg1 - dZu*eDrift, 0.);
    vf.SetArea(-tpcX/2, 0, tpcX/2, dZp + dZ12 + dZ23 + dZu);
    vf.PlotContour();
    

    // simulating avalanche of an electron.
    ViewDrift driftView;

    Sensor sensor;
    sensor.SetArea(-tpcX / 2, -tpcY / 2, 0., tpcX / 2, tpcY / 2, dZp + dZ12 + dZ23 + dZu);
    sensor.AddComponent(componentGem.get());

    // drift of ions
    AvalancheMC drift;
    drift.EnablePlotting(&driftView);
    drift.SetSensor(&sensor);
    // drift.EnableMagneticField(true);
    drift.SetCollisionSteps(2.e-4);

    AvalancheMicroscopic aval;
    aval.EnablePlotting(&driftView);
    aval.SetSensor(&sensor);
    // set the number of steps to be skipped when plotting.
    aval.SetCollisionSteps(100);

    // avalanching an electron
    const double x0 = 0.;
    const double y0 = 0.;
    const double z0 = dZp + dZ12 + dZ23 + dZu * 0.999; 
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
    for (unsigned int j = 0; j < np; ++j) {
      aval.GetElectronEndpoint(j, xe1, ye1, ze1, te1, e1, 
                                  xe2, ye2, ze2, te2, e2, status);
      drift.DriftIon(xe1, ye1, ze1, te1);
      drift.GetIonEndpoint(0, xi1, yi1, zi1, ti1, 
                              xi2, yi2, zi2, ti2, status);
    }
    std::cout << " The size of avalanche : " << ne << "\n";

    TCanvas *cd1 = new TCanvas("cd1", "Drift line of GEM1", 600, 600);cd1->SetLeftMargin(0.16);
    TCanvas *cd2 = new TCanvas("cd2", "Drift line of GEM2", 600, 600);cd2->SetLeftMargin(0.16);
    TCanvas *cd3 = new TCanvas("cd3", "Drift line of GEM3", 600, 600);cd3->SetLeftMargin(0.16);
    TCanvas *cd4 = new TCanvas("cd4", "Drift line", 600, 600);cd4->SetLeftMargin(0.16);

    ViewFEMesh meshView;
    meshView.SetComponent(componentGem.get());
    // x-z projection.
    meshView.SetPlaneXZ();
    // Set the color of the kapton.
    meshView.SetFillMesh(true);
    meshView.SetColor(0, kGray);
    meshView.SetColor(3, kYellow + 3);
    meshView.SetColor(6, kYellow + 3);
    meshView.SetColor(9, kYellow + 3);
    meshView.SetViewDrift(&driftView);
    meshView.EnableAxes();

    meshView.SetCanvas(cd1);
    meshView.SetArea(-1.5 * pitch, dZp + dZ12 + dZ23 - 3 * tD, 1.5 * pitch, dZp + dZ12 + dZ23 + 3 * tD);
    meshView.Plot();

    meshView.SetCanvas(cd2);
    meshView.SetArea(-1.5 * pitch, dZp + dZ12 - 3 * tD, 1.5 * pitch, dZp + dZ12 + 3 * tD);
    meshView.Plot();

    meshView.SetCanvas(cd3);
    meshView.SetArea(-1.5 * pitch, dZp - 3 * tD, 1.5 * pitch, dZp + 3 * tD);
    meshView.Plot();

    meshView.SetCanvas(cd4);
    meshView.SetArea(-tpcX/2, 0, tpcX/2, dZp + dZ12 + dZ23 + dZu);
    meshView.Plot();

    app.Run(true);
}