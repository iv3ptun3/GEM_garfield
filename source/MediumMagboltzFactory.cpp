#include "MediumMagboltzFactory.hpp"

using namespace Garfield;
using namespace std;

std::unordered_map<std::string, std::string> MediumMagboltzFactory::ionMobilityDirs = {
    {"He", "IonMobility_He+_He.txt"}, {"Ar", "IonMobility_Ar+_Ar.txt"}, {"Ne", "IonMobility_Ne+_Ne.txt"}
};

std::unique_ptr<Garfield::MediumMagboltz> MediumMagboltzFactory::createGasMixture(const std::string &medium, double fracMedium,
    const std::string &quenching, double fracQuenching)
{
    
    unique_ptr<MediumMagboltz> gas(new MediumMagboltz());
    try {
        if(!checkFraction(fracMedium, fracQuenching))
            throw MediumMagboltzFactoryException("The sum of fractions is not equal to 100%%.");
        gas->SetComposition(medium, fracMedium, quenching, fracQuenching);
        gas->SetTemperature(293.15);
        gas->LoadIonMobility(getIonMobilityDir(medium));
        gas->SetMaxElectronEnergy(200);
    }
    catch(MediumMagboltzFactoryException const &e) {
        throw e;
    }
    return move(gas);
}

bool MediumMagboltzFactory::ionMobilityDirExists(const std::string &ionName)
{
    return ionMobilityDirs.find(ionName) != ionMobilityDirs.end();
}

std::string MediumMagboltzFactory::getIonMobilityDir(const std::string &ionName)
{
    char *path = std::getenv("GARFIELD_INSTALL");
    if(!ionMobilityDirExists(ionName))
        throw MediumMagboltzFactoryException("There is no ion mobility data for " + ionName);
    else if(path == NULL)
        throw MediumMagboltzFactoryException("Cannot get an environmental varialbe \"GARFIELD_INSTALL\".");
    return std::string(path) + "/share/Garfield/Data/" + ionMobilityDirs[ionName];
}

bool MediumMagboltzFactory::checkFraction(double frac1, double frac2)
{
    return (frac1 + frac2 - 100)*(frac1 + frac2 - 100)/(100*100) < 1e-6;
}