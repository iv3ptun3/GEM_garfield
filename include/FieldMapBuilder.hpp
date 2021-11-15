#ifndef FIELDMAPBUILDER_HPP
#define FIELDMAPBUILDER_HPP

#include "Garfield/ComponentConstant.hh"
#include "Garfield/ComponentElmer.hh"
#include "Garfield/MediumMagboltz.hh"

#include <string>

using namespace Garfield;

// this builder class builds Garfield::Medium and Garfield::Component instants loading parameter file and .
class FieldMapBuilder{
    public:
        FieldMapBuilder():fGas(nullptr){}
        
        virtual ~FieldMapBuilder()
        {
            if(fGas != nullptr)
                delete fGas;
        }

        MediumMagboltz* getGas(){return fGas;}
        MediumMagboltz* initGas();
        // return ptr to ComponentFieldMap of drift field or tripple GEM field.
        ComponentConstant* buildDriftFieldMap();
        ComponentElmer* buildGemFieldMap(const bool print = true);
        ComponentConstant* buildMagneticField();
    private:
    private:
        MediumMagboltz* fGas;
};

#endif // FIELDMAPBUILDER_HPP