#ifndef RUNANALYZER_HPP
#define RUNANALYZER_HPP

#include "analyzer/IEventAnalyzer.hpp"
#include "Exception.hpp"

#include "TFile.h"

#include <memory>
#include <string>

class RunAnalyzer {
    public:
    RunAnalyzer() : saveByEvent(false), file(nullptr), eventAnalyzer(nullptr){}
    
    void SaveByEvent(bool saveByEvent_ = true)
    {
        saveByEvent = saveByEvent_;
    }

    void Open(const std::string &fileName)
    {
        Close();
        file = std::make_unique<TFile>(fileName.c_str(), "RECREATE");
        if(file == nullptr)
            throw Exception("Failed to open root file " + fileName);
    }
    void SetEventAnalyzer(IEventAnalyzer *_eventAnalyzer)
    {
        if(!IsOpened())
            throw Exception("Root file must be opened before setting a pointer to event analyzer.");
        file->cd();
        eventAnalyzer = _eventAnalyzer;
        eventAnalyzer->Init();
    }

    void ProcessEvent()
    {
        if(eventAnalyzer == nullptr)
            throw Exception("Event Analyzer is not set.");
        eventAnalyzer->AnalyzeEvent();
        if(saveByEvent)
            file->Write(nullptr, TObject::kOverwrite);
    }
    void Write()
    {
        if(!IsOpened())
            throw Exception("Root file not opened.");
        file->Write(nullptr, TObject::kOverwrite);
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
    bool saveByEvent;
    std::unique_ptr<TFile> file;
    IEventAnalyzer *eventAnalyzer;
};

#endif // RUNANALYZER_HPP
