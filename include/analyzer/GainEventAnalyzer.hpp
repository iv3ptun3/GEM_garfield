#ifndef GAINEVENTANALYZER_HPP
#define GAINEVENTANALYZER_HPP

#include "analyzer/IEventAnalyzer.hpp"

#include "Garfield/AvalancheMicroscopic.hh"

#include "TTree.h"
#include "TH1D.h"

#include <memory>

class GainEventAnalyzer : public IEventAnalyzer {
    public:
    GainEventAnalyzer(Garfield::AvalancheMicroscopic *aval);

    protected:
    virtual void Init() override;
    virtual void AnalyzeEvent() override;

    private:
    void Update();

    private:
    const Garfield::AvalancheMicroscopic *aval;
    std::unique_ptr<TTree> treeGain;
    std::unique_ptr<TH1D> xHist, yHist, tHist;

    int gain;
    int ne, ni;
    long long evtId;
};

#endif // GAINEVENTANALYZER_HPP
