#ifndef IEVENTANALYZER_HPP
#define IEVENTANALYZER_HPP

#include "analyzer/IEventAnalyzer.hpp"

#include "TFile.h"

#include <memory>
#include <string>

class RunAnalyzer;

// Interface for event unit analysis
class IEventAnalyzer {
    friend class RunAnalyzer;
    
    public:
    IEventAnalyzer(){}
    
    protected:
    virtual void Init() = 0;
    virtual void AnalyzeEvent() = 0;
};

#endif // IEVENTANALYZER_HPP
