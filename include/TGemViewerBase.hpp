#ifndef TGEMVIEWERBASE_HPP
#define TGEMVIEWERBASE_HPP

#include "Garfield/ComponentFieldMap.hh"
#include "Garfield/ViewBase.hh"
#include "TCanvas.h"

using namespace Garfield;
// This class is base class for TGemViewerBase.
class TGemViewerBase{
    public:
        TGemViewerBase(ComponentFieldMap *fm)
        :fCanvas(nullptr), fNx(0), fNy(-1), fNz(0), fx0(0), fy0(0), fz0(0)
        {}
        ~TGemViewerBase(){};
        void setViewPlane(double fx, double fy, double fz, double x0, double y0, double z0)
        {
            fNx = fx;
            fNy = fy;
            fNz = fz;
            fx0 = x0;
            fy0 = y0;
            fz0 = z0;
        }
        void setCanvas(TPad* c){fCanvas = c;}
        // abstract method
        virtual void plot(double xmin, double ymin, double xmax, double ymax) = 0;
    protected:
        TPad *fCanvas;
        double fNx, fNy, fNz, fx0, fy0, fz0;
};

#endif // TGemViewerBase_HPP