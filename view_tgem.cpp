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

#include "TGemViewerField.hpp"
#include "TGemViewerDrift.hpp"
#include "ParManager.hpp"
#include "FieldMapBuilder.hpp"

using namespace Garfield;

int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        std::cout << "Usage : view_tgem [parameter file]" << std::endl;
        return 0;
    }
    // Initializing global managers.
    auto parMan = ParManager::getInstance();
    parMan->initPars(argv[1]);
    // parMan->listPars();

    TApplication app("app", &argc, argv);

    // tripple gem dimension
    const double pitch = parMan->getParD("PITCH");
    const double tD = parMan->getParD("T_DIEL");
    const double dZp = parMan->getParD("DZ_PADPLANE");
    const double dZ12 = parMan->getParD("DZ_GEM12"); 
    const double dZ23 = parMan->getParD("DZ_GEM23"); 
    const double dZe = parMan->getParD("DZ_UPPERPLANE");
    // voltage
    const double dvg1 = parMan->getParD("DV_GEM1");
    const double dvg2 = parMan->getParD("DV_GEM2");
    const double dvg3 = parMan->getParD("DV_GEM3");
    // electric field density
    const double eTrans = parMan->getParD("E_TRANS");
    const double eDrift = parMan->getParD("E_DRIFT");
    const double eInduction = parMan->getParD("E_INDUCTION");
    // get a field map from the builder
    FieldMapBuilder* fmBuilder = new FieldMapBuilder();
    fmBuilder->initGas();
    ComponentElmer* fm = fmBuilder->buildGemFieldMap();

    TGemViewerField *fv = new TGemViewerField(fm);
    TCanvas *cf1 = new TCanvas("cf1", "Potential plot of GEM1", 600, 600);
    TCanvas *cf2 = new TCanvas("cf2", "Potential plot of GEM2", 600, 600);
    TCanvas *cf3 = new TCanvas("cf3", "Potential plot of GEM3", 600, 600);
    fv->setNumberOfContour(104);
    cf1->SetLeftMargin(0.16);
    cf2->SetLeftMargin(0.16);    
    cf3->SetLeftMargin(0.16);    

    fv->setCanvas(cf1);
    fv->setVoltageRange(
        -dZp*eInduction - dvg1 - dZ12*eTrans - dvg2 - dZ23*eTrans - 1.5*dvg3, 
        -dZp*eInduction - dvg1 - dZ12*eTrans - dvg2 - dZ23*eTrans + 1.5*dvg3);
    fv->plot(-1.5*pitch, dZp + dZ12 + dZ23 - 3*tD, 1.5*pitch, dZp + dZ12 + dZ23 + 3*tD);
    
    fv->setCanvas(cf2);
    fv->setVoltageRange(
        -dZp*eInduction - dvg1 - dZ12*eTrans - 1.5*dvg2, 
        -dZp*eInduction - dvg1 - dZ12*eTrans + 1.5*dvg2);
    fv->plot(-1.5*pitch, dZp + dZ12 - 3*tD,1.5*pitch, dZp + dZ12 + 3*tD);
    
    fv->setCanvas(cf3);
    fv->setVoltageRange(
        -dZp*eInduction - 1.5*dvg1, 
        -dZp*eInduction + 1.5*dvg1);
    fv->plot(-1.5*pitch, dZp - 3*tD, 1.5*pitch, dZp + 3*tD);   
    fv->setOption("e");
    TCanvas *cd1 = new TCanvas("cd1", "Drift line of GEM1", 600, 600);
    TCanvas *cd2 = new TCanvas("cd2", "Drift line of GEM2", 600, 600);
    TCanvas *cd3 = new TCanvas("cd3", "Drift line of GEM3", 600, 600);
    TCanvas *cd4 = new TCanvas("cd4", "Drift line", 600, 600);

    cd1->SetLeftMargin(0.16);    
    cd2->SetLeftMargin(0.16);    
    cd3->SetLeftMargin(0.16);    
    cd4->SetLeftMargin(0.16);    

    TGemViewerDrift *dv = new TGemViewerDrift(fm);
    dv->setAvalancheArea(-3*pitch, -3*sqrt(3.)*pitch, 0, 3*pitch, 3*sqrt(3)*pitch, dZp + dZ12 + dZ23 + dZe);
    dv->avalancheElectron(0, 0, dZp + dZ12 + dZ23 + dZe/2, 0.1, 0., 2);
    dv->setCanvas(cd1);
    dv->plot(-1.5*pitch, dZp + dZ12 + dZ23 - 3*tD, 1.5*pitch, dZp + dZ12 + dZ23 + 3*tD);
    dv->setCanvas(cd2);
    dv->plot(-1.5*pitch, dZp + dZ12 - 3*tD,1.5*pitch, dZp + dZ12 + 3*tD);
    dv->setCanvas(cd3);
    dv->plot(-1.5*pitch, dZp - 3*tD, 1.5*pitch, dZp + 3*tD); 
    dv->setCanvas(cd4);
    dv->plot(-10*pitch, 0, 10*pitch, dZp + dZ12 + dZ23 + dZe);
    app.Run(true);
}
