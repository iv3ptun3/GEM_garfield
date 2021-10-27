#ifndef TGEMVIEWERFIELD_HPP
#define TGEMVIEWERFIELD_HPP

#include <string>

#include "Garfield/ComponentFieldMap.hh"
#include "Garfield/ViewField.hh"
#include "TGemViewerBase.hpp"

using namespace Garfield;
// This class draw voltage line of given component field map.
class TGemViewerField : public TGemViewerBase{
    public:
        TGemViewerField(ComponentFieldMap *fm);
        virtual ~TGemViewerField();
        void setNumberOfContour(int n){((ViewField*)fFieldViewer)->SetNumberOfContours(n);}
        void setVoltageRange(double vmin, double vmax){fFieldViewer->SetVoltageRange(vmin, vmax);}
        // option : quantity to be plotted(will be passed to Garfield::ViewField::PlotContour()).
        void setOption(std::string opt){fOpt = opt;}
        // potential : "v", "voltage", "p"
        // magnitude  of E-field : "e", "field"
        // i-th component of electric field : "ei"
        virtual void plot(double xmin, double ymin, double xmax, double ymax);

    private:
        ViewField* fFieldViewer;
        std::string fOpt;
};

#endif // TGEMVIEWERFIELD_HPP