#ifndef FIELDMAPBUILDER_HPP
#define FIELDMAPBUILDER_HPP

#include "Garfield/ComponentAnalyticField.hh"
#include "Garfield/ComponentElmer.hh"
#include "Garfield/MediumMagboltz.hh"

#include <string>

using namespace Garfield;

// this builder class builds Garfield::Medium and Garfield::Component instants loading parameter file and .
class FieldMapBuilder{
    public:
        FieldMapBuilder():fGas(nullptr){}
        
        ~FieldMapBuilder()
        {
            if(fGas != nullptr)
                delete fGas;
        }
        
        MediumMagboltz* getGas(){return fGas;}
        MediumMagboltz* initGas();
        // return ptr to ComponentFieldMap of drift field or tripple GEM field.
        ComponentAnalyticField* buildDriftFieldMap();
        ComponentElmer* buildGemFieldMap();
    private:
    private:
        MediumMagboltz* fGas;
};

#endif // FIELDMAPBUILDER_HPP