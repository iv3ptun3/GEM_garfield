#include "analyzer/GainEventAnalyzer.hpp"
#include <iomanip>
#include <iostream>
#include <TDirectory.h>

using namespace Garfield;
using namespace std;

GainEventAnalyzer::GainEventAnalyzer(AvalancheMicroscopic *aval_)
    : IEventAnalyzer(), aval(aval_)
{
}

void GainEventAnalyzer::Init()
{
    evtId = 0;
    treeGain = make_unique<TTree>("gain", "gain");
    treeGain->Branch("ne", &ne, "ne/I");
    
    xHist = make_unique<TH1D>("xHist", "x position", 10000, -0.2, 0.2);
    yHist = make_unique<TH1D>("yHist", "y position", 10000, -0.2, 0.2);
    tHist = make_unique<TH1D>("tHist", "arrival time", 10000, 0., 10.);
}

void GainEventAnalyzer::AnalyzeEvent()
{
    gain = 0;
    unsigned int np;
    int ne, ni;
    double xe1, ye1, ze1, te1, e1;
    double xe2, ye2, ze2, te2, e2;
    int status;

    aval->GetAvalancheSize(ne, ni);
    np = aval->GetNumberOfElectronEndpoints();
    for(unsigned int i = 0;i < np;++i)
    {
        aval->GetElectronEndpoint(i,
            xe1, ye1, ze1, te1, e1,
            xe2, ye2, ze2, te2, e2, 
            status);
        if(ze2 > 1e-2)
            continue;
        else
        {
            ++gain;
            xHist->Fill(xe2);
            yHist->Fill(ye2);
            tHist->Fill(te2);
        }
    }
    treeGain->Fill();
    treeGain->AutoSave("SaveSelf");
    std::cout << evtId  << " : " << gain << "\n";
}

void GainEventAnalyzer::Write(TDirectory *dir)
{
    treeGain->SetDirectory(dir);
    xHist->SetDirectory(dir);
    yHist->SetDirectory(dir);
    tHist->SetDirectory(dir);
    dir->Write();
}