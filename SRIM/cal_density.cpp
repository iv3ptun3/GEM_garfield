// ROOT macro to calculate gas density.
// input : Atomic masses, fractions and presures of two gas mixture.
#include <iostream>

int cal_density()
{
    double mGas1 = 0, mGas2 = 0., fGas1 = 0, fGas2 = 0;
    double pressure = 0;
    constexpr double air_mass = 28.79;
    constexpr double air_density = 0.001225;
    std::cout << "Enter atomic mass of ionizable gas component(mol/g)           : ";
    std::cin >> mGas1;
    std::cout << "Enter atomic mass of quenching gas component(mol/g)           : ";
    std::cin >> mGas2;
    std::cout << "Enter fraction of ionizable gas component(percentage)         : ";
    std::cin >> fGas1;
    std::cout << "Enter fraction mass of ionizable gas component(percentage)    : ";
    std::cin >> fGas2;
    if ((fGas1 + fGas2 - 100.) * (fGas1 + fGas2 - 100.) > 1e-6)
    {
        std::cout << "The sum of gas fractions is not close enough to 100%!\n";
        return -1;
    }
    std::string s = "Y";
    while (s == "Y" || s == "y")
    {
        std::cout << "Enter gas pressure(atm)                           : ";
        std::cin >> pressure;
        std::cout << "Mass density of gas mixture in " << pressure*760 << " torr is " << pressure * air_density * (mGas1 * fGas1 + mGas2 * fGas2) / ((fGas1 + fGas2) * air_mass) << " g/cm^3.\n";
        while (true)
        {
            std::cout << "continue?(Enter Y or N) : ";
            std::cin >> s;
            if(s == "Y" || s == "N" || s == "y" || s == "n")
                break;
        }
    }
    return 1;
}