// definition of TGemViewerField class method

#include "TGemViewerField.hpp"

TGemViewerField::TGemViewerField(ComponentFieldMap *fm):
TGemViewerBase(fm), fFieldViewer(nullptr), fOpt("v")
{
    fFieldViewer = new ViewField();
    fFieldViewer->SetComponent(fm);
    fFieldViewer->SetVoltageRange(0, 100);
}

TGemViewerField::~TGemViewerField()
{
    if(fFieldViewer != nullptr)
        delete fFieldViewer;
}

void TGemViewerField::plot(double xmin, double ymin, double xmax, double ymax)
{
    fFieldViewer->SetPlane(fNx, fNy, fNz, fx0, fy0, fz0);
    fFieldViewer->SetArea(xmin, ymin, xmax, ymax);
    fFieldViewer->SetCanvas(fCanvas);
    fFieldViewer->PlotContour(fOpt);
}