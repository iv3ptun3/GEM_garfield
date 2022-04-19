#ifndef GAINEVENTANALYZER_HPP
#define GAINEVENTANALYZER_HPP

#include "analyzer/IEventAnalyzer.hpp"

#include "Garfield/AvalancheMicroscopic.hh"

#include "TTree.h"
#include "TH1D.h"

#include <memory>

class GainEventAnalyzer : public IEventAnalyzer {
    private:
    struct Response{
        long long gain;
        float x_m, y_m, x_s, y_s;
    };

    public:
    GainEventAnalyzer(Garfield::AvalancheMicroscopic *aval);

    protected:
    virtual void Init() override;
    virtual void AnalyzeEvent() override;

    private:
    void Update();

    private:
    const Garfield::AvalancheMicroscopic *aval;
    TTree *treeGain;
    TH1D *xHist, *yHist, *tHist;

    Response response;

    long long evtId, iEvtId; // intersted event
    

};

#endif // GAINEVENTANALYZER_HPP
