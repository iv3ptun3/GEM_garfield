#include <cstdlib>
#include <string>

#include "FieldMapBuilder.hpp"
#include "ParManager.hpp"
#include "Errors.hpp"

MediumMagboltz* FieldMapBuilder::initGas()
{
    auto parMan = ParManager::getInstance();
    if(fGas != nullptr)
        delete fGas;
    fGas = new MediumMagboltz();
    fGas->SetComposition(
        parMan->getParS("GAS_MEDIUM"), parMan->getParD("GAS_MEDIUM_FRAC"),
        parMan->getParS("GAS_QUENCHING"), parMan->getParD("GAS_QUENCHING_FRAC"));
    fGas->SetPressure(parMan->getParD("GAS_PRESSURE"));
    fGas->SetTemperature(parMan->getParD("GAS_TEMPERATURE"));
    fGas->Initialise(true);
    fGas->EnablePenningTransfer(parMan->getParD("GAS_PENNING_EFF"), parMan->getParD("GAS_PENNING_LAMB"),
        parMan->getParS("GAS_MEDIUM"));
    const std::string path = std::getenv("GARFIELD_INSTALL");
    fGas->LoadIonMobility(path + parMan->getParS("GAS_ION_MOBILITY"));
    return fGas;
}

ComponentAnalyticField* FieldMapBuilder::buildDriftFieldMap()
{
    auto parMan = ParManager::getInstance();
    if(fGas == nullptr)
    {
        printError("FieldMapBuilder", "buildDriftFieldMap()", "fGas is a null pointer.");
        return nullptr;
    }
    ComponentAnalyticField* fm = new ComponentAnalyticField();
    setMagneticField(fm, parMan->getParD("B_X"), parMan->getParD("B_Y"), parMan->getParD("B_Z"));
    return fm;
}

ComponentElmer* FieldMapBuilder::buildGemFieldMap()
{
    auto parMan = ParManager::getInstance();
    std::string path = parMan->getParS("SCRIPT_NAME");
    ComponentElmer* fm = new ComponentElmer(
        path + "/mesh.header", path + "/mesh.elements", path + "/mesh.nodes",
        path + "/dielectrics.dat", path + "/" + path + ".result", "cm");
    fm->EnablePeriodicityX();
    fm->EnablePeriodicityY();
    // Associate the gas with the corresponding field map material.
    const unsigned int nMaterials = fm->GetNumberOfMaterials();
    for (unsigned int i = 0; i < nMaterials; ++i)
    {
        const double eps = fm->GetPermittivity(i);
        if (eps == 1.)
            fm->SetMedium(i, fGas);
    }
    setMagneticField(fm, parMan->getParD("B_X"), parMan->getParD("B_Y"), parMan->getParD("B_Z"));
    fm->PrintMaterials();
    return fm;
}