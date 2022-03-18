#include <cstdlib>
#include <iostream>

#include "Garfield/MediumMagboltz.hh"

#include "MediumMagboltzFactory.hpp"

using namespace Garfield;

int main(int argc, char *argv[])
{
    if (argc != 6)
    {
        std::cout << "Usage : gen_gasfile <arg1> <arg2> <arg3> <arg4> <arg5>" << std::endl;
        std::cout << "\t<arg1> : Ionizable gas" << std::endl;
        std::cout << "\t<arg2> : Fraction of the gas in percentage" << std::endl;
        std::cout << "\t<arg3> : Quenching gas" << std::endl;
        std::cout << "\t<arg4> : Fraction of the gas in percentage" << std::endl;
        std::cout << "\t<arg5> : Gas pressure in mmHg" << std::endl;
        return 0;
    }
    const char *gas1 = argv[1], *gas2 = argv[3];
    const double frac1 = atof(argv[2]), frac2 = atof(argv[4]);
    const double pressure = atof(argv[5]);

    const size_t nE = 10;
    const double emin = 100.;
    const double emax = 2000.;
    // Flag to request logarithmic spacing.
    constexpr bool useLog = false;
    // Range of magnetic fields [Tesla]
    const size_t nB = 10;
    const double bmin = 0.;
    const double bmax = 3.;
    
    // Range of angles [rad]
    const size_t nA = 1;
    const double amin = 0.;
    const double amax = HalfPi;

    std::unique_ptr<MediumMagboltz> gas
        = MediumMagboltzFactory::createGasMixture(gas1, frac1, gas2, frac2);
    gas->SetPressure(pressure);
    gas->SetFieldGrid(emin, emax, nE, useLog, bmin, bmax, nB, amin, amax, nA); 
    gas->GenerateGasTable(10, false);
    char gasFileName[256];
    sprintf(gasFileName, "%s_%02d_%s_%02d_%03d.gas", gas1, (int)frac1, gas2, (int)frac2, (int)pressure);
    gas->WriteGasFile(gasFileName);
    return 0;
}