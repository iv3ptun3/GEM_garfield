// definition of TGemViewerDrift class method

#include "TGemViewerDrift.hpp"

TGemViewerDrift::TGemViewerDrift(ComponentFieldMap *fm) : TGemViewerBase(fm), fDrift(nullptr), fAval(nullptr),
                                                          fDriftView(nullptr), fMeshView(nullptr), fSensor(nullptr),
                                                          fEnableDriftIons(true)
{
    fSensor = new Sensor();
    fSensor->AddComponent(fm);
    fSensor->SetArea(-1., -1., -1., 1, 1, 1);

    fDriftView = new ViewDrift();

    fDrift = new AvalancheMC();
    fDrift->SetSensor(fSensor);
    fDrift->SetDistanceSteps(2.e-4);
    fDrift->EnablePlotting(fDriftView);

    fAval = new AvalancheMicroscopic();
    fAval->SetSensor(fSensor);
    fAval->EnablePlotting(fDriftView);

    fMeshView = new ViewFEMesh();
    fMeshView->SetComponent(fm);
    fMeshView->SetPlane(fNx, fNy, fNz, fx0, fy0, fz0);
    fMeshView->SetFillMesh(true);
    fMeshView->SetColor(0, kGray);
    fMeshView->SetColor(3, kYellow + 3);
    fMeshView->SetColor(6, kYellow + 3);
    fMeshView->SetColor(9, kYellow + 3);
    fMeshView->EnableAxes();
}

TGemViewerDrift::~TGemViewerDrift()
{
    if (fSensor != nullptr)
        delete fSensor;
    if (fDrift != nullptr)
        delete fDrift;
    if (fAval != nullptr)
        delete fAval;
    if (fMeshView != nullptr)
        delete fMeshView;
}

void TGemViewerDrift::avalancheElectron(double x0, double y0, double z0, double e0, double t0, const int nEvents)
{
    for (unsigned int i = 0; i < nEvents; ++i)
    {
        // Randomize the initial position.
        fAval->AvalancheElectron(x0, y0, z0, t0, e0, 0., 0., 0.);
        int ne = 0, ni = 0;
        fAval->GetAvalancheSize(ne, ni);
        std::cout << i << "/" << nEvents << " " << ne << "\n";
        const unsigned int np = fAval->GetNumberOfElectronEndpoints();
        double xe1, ye1, ze1, te1, e1;
        double xe2, ye2, ze2, te2, e2;
        double xi1, yi1, zi1, ti1;
        double xi2, yi2, zi2, ti2;
        int status;
        for (unsigned int j = 0; j < np; ++j)
        {
            fAval->GetElectronEndpoint(j, xe1, ye1, ze1, te1, e1,
                                       xe2, ye2, ze2, te2, e2, status);
            if(fEnableDriftIons)
            {    
                fDrift->DriftIon(xe1, ye1, ze1, te1);
                fDrift->GetIonEndpoint(0, xi1, yi1, zi1, ti1,
                                        xi2, yi2, zi2, ti2, status);
            }
        }
    }
}

void TGemViewerDrift::setMaterialColors(std::vector<int> matIds, int color)
{
    for(auto m : matIds)
        fMeshView->SetColor(m, color);
}

void TGemViewerDrift::plot(double xmin, double ymin, double xmax, double ymax)
{
    fMeshView->SetViewDrift(fDriftView);
    fMeshView->SetArea(xmin, xmin, ymin, xmax, xmax, ymax);
    fMeshView->SetCanvas(fCanvas);
    fMeshView->Plot();
}