#ifndef TGEMVIEWERDrift_HPP
#define TGEMVIEWERDrift_HPP

#include "Garfield/ComponentFieldMap.hh"
#include "Garfield/MediumGas.hh"
#include "Garfield/ViewFEMesh.hh"
#include "Garfield/Sensor.hh"
#include "Garfield/AvalancheMicroscopic.hh"
#include "Garfield/AvalancheMC.hh"

#include "TGemViewerBase.hpp"
// This class process electron avalanche and draw their drift lines on the mesh.
using namespace Garfield;

class TGemViewerDrift : public TGemViewerBase{
    public:
        TGemViewerDrift(ComponentFieldMap *fm);
        ~TGemViewerDrift();
        void setAvalancheArea(double xmin, double ymin, double zmin, double xmax, double ymax, double zmax)
        {
            fSensor->SetArea(xmin, ymin, zmin, xmax, ymax, zmax);
        }
        void enableDriftIons(bool dIons){fEnableDriftIons = dIons;}
        void setMaterialColors(std::vector<int> matIds, int color);
        void avalancheElectron(double x0, double y0, double z0, double e0, double t0 = 0, const int nEvent = 1);
        virtual void plot(double xmin, double ymin, double xmax, double ymax);
    private:
        AvalancheMC* fDrift;
        AvalancheMicroscopic* fAval;
        ViewDrift* fDriftView;
        ViewFEMesh* fMeshView;
        Sensor* fSensor;
        bool fEnableDriftIons;
};

#endif // TGemViewerRIFT_HPP