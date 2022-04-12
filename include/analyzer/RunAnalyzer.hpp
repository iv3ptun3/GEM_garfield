#ifndef RUNANALYZER_HPP
#define RUNANALYZER_HPP

#include "analyzer/IEventAnalyzer.hpp"
#include "Exception.hpp"

#include "TFile.h"

#include <memory>
#include <string>

class RunAnalyzer {
    public:
    RunAnalyzer() : file(nullptr), eventAnalyzer(nullptr){}
    void Open(const std::string &fileName)
    {
        Close();
        file = std::make_unique<TFile>(fileName.c_str(), "RECREATE");
        if(file == nullptr)
            throw Exception("Failed to open root file " + fileName);
    }
    void SetEventAnalyzer(IEventAnalyzer *_eventAnalyzer)
    {
        eventAnalyzer = _eventAnalyzer;
        eventAnalyzer->Init();
    }

    void ProcessEvent()
    {
        if(eventAnalyzer == nullptr)
            throw Exception("Event Analyzer is not set.");
        eventAnalyzer->AnalyzeEvent();
    }
    void Write()
    {
        if(!IsOpened())
            throw Exception("Root file not opened.");
        eventAnalyzer->Write(file.get());
    }

    bool IsOpened() const
    {
        if(file == nullptr)
            return false;
        else
            return file->IsOpen();
    }

    void Close()
    {
        if(IsOpened())
            file->Close();
    }

    private:
    std::unique_ptr<TFile> file;
    IEventAnalyzer *eventAnalyzer;
};

#endif // RUNANALYZER_HPP
