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
    iEvtId = 0;
    treeGain = new TTree("tree", "electron statistics");
    treeGain->Branch("response", &response, "ne/l:x_m/F:y_m:x_s:y_s");
    xHist = new TH1D("xHist", "x position", 10000, -0.2, 0.2);
    yHist = new TH1D("yHist", "y position", 10000, -0.2, 0.2);
    tHist = new TH1D("tHist", "arrival time", 10000, 0., 10.);
}

void GainEventAnalyzer::AnalyzeEvent()
{
    unsigned int np;
    int ne, ni;
    double xe1, ye1, ze1, te1, e1;
    double xe2, ye2, ze2, te2, e2;
    int status;

    response = {0, -9999., -9999., -9999., -9999.};
    double xSum = 0, ySum = 0;
    double xxSum = 0, yySum = 0;

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
            xSum += xe2, ySum += ye2;
            xxSum += xe2*xe2, yySum += ye2*ye2;
            ++response.gain;
            xHist->Fill(xe2);
            yHist->Fill(ye2);
            tHist->Fill(te2);
        }
    }
    if(response.gain > 0)
    {
        response.x_m = xSum/response.gain;
        response.y_m = ySum/response.gain;
        double gg = (double)response.gain*response.gain;
        response.x_s = sqrt(xxSum/response.gain - xSum*xSum/gg);
        response.y_s = sqrt(yySum/response.gain - ySum*ySum/gg);
        ++iEvtId;
    }
    treeGain->Fill();
    ++evtId;
    cout << "# of Events => Total : " << setw(10) <<evtId <<
        setw(20) << "Interested : " << iEvtId <<
        setw(20) << "Gain : " << response.gain << endl;
}