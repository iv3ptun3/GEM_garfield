#include <cstdlib>
#include <iostream>
#include <vector>

#include "Garfield/MediumMagboltz.hh"

#include "MediumMagboltzFactory.hpp"

using namespace Garfield;

std::vector<double> ElectricFieldGrid();
std::vector<double> MagneticFieldGrid();

int main(int argc, char *argv[])
{
    std::unique_ptr<MediumMagboltz> gas;
    if(argc < 6 || argc > 8)
    {
        std::cout << "Usage : gen_gasfile <arg1> <arg2> <arg3> <arg4> <arg5> [<arg6> <arg7>]" << std::endl;
        std::cout << "\t<arg1> : Ionizable gas" << std::endl;
        std::cout << "\t<arg2> : Fraction of the gas in percentage" << std::endl;
        std::cout << "\t<arg3> : Quenching gas" << std::endl;
        std::cout << "\t<arg4> : Fraction of the gas in percentage" << std::endl;
        std::cout << "\t<arg5> : Gas pressure in mmHg" << std::endl;
        std::cout << "Optionals" << std::endl;
        std::cout << "\t<arg6> : Penning transfer probabiliy.(optional)" << std::endl;
        std::cout << "\t<arg7> : Mean free path of Penning effect.(optional)" << std::endl;
        return 0;
    }

    const char *gas1 = argv[1], *gas2 = argv[3];
    const double frac1 = atof(argv[2]), frac2 = atof(argv[4]);
    const double pressure = atof(argv[5]);
    if(argc == 6)
        gas = MediumMagboltzFactory::createGasMixture(gas1, frac1, gas2, frac2);
    else if(argc == 7)
        gas = MediumMagboltzFactory::createGasMixture(gas1, frac1, gas2, frac2, atof(argv[6]), 0.);
    else
        gas = MediumMagboltzFactory::createGasMixture(gas1, frac1, gas2, frac2, atof(argv[6]), atof(argv[7]));

    std::vector<double> eFieldGrid = ElectricFieldGrid();
    std::vector<double> bFieldGrid = MagneticFieldGrid();
    std::vector<double> aGrid = {0.};


    gas->SetPressure(pressure);
    gas->SetFieldGrid(eFieldGrid, bFieldGrid, aGrid);
    gas->GenerateGasTable(10, false);
    char gasFileName[256];
    sprintf(gasFileName, "%s_%02d_%s_%02d_%04d.gas", gas1, (int)frac1, gas2, (int)frac2, (int)pressure);
    gas->WriteGasFile(gasFileName);
    return 0;
}

std::vector<double> ElectricFieldGrid()
{
    std::vector<double> fieldGrid;
    // Linear range from 100 V/cm to 1900 V/cm 
    for(int i = 0;i < 10;++i)
        fieldGrid.push_back(100. + 200.*i);
    // Roughly logarithmic range to 200000.
    fieldGrid.push_back(5000.);
    fieldGrid.push_back(10000.);
    fieldGrid.push_back(20000.);
    fieldGrid.push_back(50000.);
    fieldGrid.push_back(100000.);
    fieldGrid.push_back(200000.);
    return fieldGrid;
}

std::vector<double> MagneticFieldGrid()
{
    std::vector<double> fieldGrid;
    // Linear range from 0 T to 3.0 T 
    for(int i = 0;i < 7;++i)
        fieldGrid.push_back(0.5*i);
    return fieldGrid;
}