#include <cstdlib>
#include <string>
#include "Garfield/SolidBox.hh"
#include "Garfield/GeometrySimple.hh"

#include "ComponentFactory.hpp"

using namespace Garfield;
using namespace std;

std::unique_ptr<ComponentElmer> ComponentFactory::createElmer(MediumMagboltz *medium, const std::string &name)
{
    std::string path = getExeAbsPath() + name;
    std::unique_ptr<ComponentElmer> fm(new ComponentElmer(
        path + "/mesh.header", path + "/mesh.elements", path + "/mesh.nodes",
        path + "/dielectrics.dat", path + "/" + name + ".result", "cm"));
    fm->EnablePeriodicityX();
    fm->EnablePeriodicityY();
    fm->EnableConvergenceWarnings(false);
    const unsigned int nMaterials = fm->GetNumberOfMaterials();
    for(unsigned int i = 0; i < nMaterials; ++i)
    {
        const double eps = fm->GetPermittivity(i);
        if(eps == 1.)
            fm->SetMedium(i, medium);
    }
    if(!fm->IsReady())
    {
        std::string message = "ComponentElmer is not initialized properly, returning nullptr.";
        throw ComponentFactoryException(message);
    }
    return move(fm);
}

std::unique_ptr<ComponentConstant> ComponentFactory::createConstE(MediumMagboltz *medium, double Ez,
    double x0, double y0, double z0,
    double dx, double dy, double dz)
{
    SolidBox *box = new SolidBox(x0, y0, z0, dx, dy, dz);
    GeometrySimple *geo = new GeometrySimple();
    geo->AddSolid(box, medium);

    std::unique_ptr<ComponentConstant> fm(new ComponentConstant());
    fm->SetMedium(medium);
    fm->SetGeometry(geo);
    fm->SetElectricField(0, 0, Ez);
    return move(fm);
}

std::unique_ptr<ComponentConstant> ComponentFactory::createConstB(MediumMagboltz *medium, double Bz,
    double x0, double y0, double z0,
    double dx, double dy, double dz)
{
    SolidBox *box = new SolidBox(x0, y0, z0, dx, dy, dz);
    GeometrySimple *geo = new GeometrySimple();
    geo->AddSolid(box, medium);

    std::unique_ptr<ComponentConstant> fm(new ComponentConstant());
    fm->SetMedium(medium);
    fm->SetGeometry(geo);
    fm->SetMagneticField(0, 0, Bz);
    return move(fm);
}

const std::string ComponentFactory::getExeAbsPath()
{
    char buffer[1024];
    ssize_t length = readlink("/proc/self/exe", buffer, sizeof(buffer)/sizeof(char));
    buffer[length] = '\0';
    std::string path = buffer;
    return path.substr(0, path.find_last_of('/') + 1);
}