#include <cstdlib>
#include <iostream>
#include <vector>
#include <string>
#include <exception>

#include "Garfield/MediumMagboltz.hh"
#include "Garfield/ViewMedium.hh"

#include "TApplication.h"
#include "TGraph.h"
#include "TH1F.h"
#include "MediumMagboltzFactory.hpp"


using namespace Garfield;
using namespace std;

void PrintUsage()
{
    std::cout << "Usage : plot_gasfile <arg1> [-e B] [-b E] [--p] [-f e]" << std::endl;
    std::cout << "<arg1> : Name of gas file" << std::endl;
    std::cout << "\tOptions : " << std::endl;
    std::cout << "\t[-e B] : Plot electron diffusion and drift vs electric field at the given magnetic field of Bz [T]" << std::endl;
    std::cout << "\t[-b E] : Plot electron diffusion and drift vs magnetic field at the given electric field of Ez [V/cm]" << std::endl;
    std::cout << "\t[-f e] : Set output file extension.(png as default)" << std::endl;
    std::cout << "\t[--p]  : Print all canvas(false as default)." << std::endl;
}

int main(int argc, char *argv[])
{
    std::string gasFileName;
    std::string extention = "png";
    int nPlots = 0;
    std::vector<char> plotOpts;
    std::vector<double> values;
    bool print = false;

    if(argc < 2)
    {
        PrintUsage();
        return 0;
    }
    else
    {
        try
        {
            gasFileName = argv[1];
            for(int i = 2;i < argc;i += 2)
            {
                if(strcmp(argv[i], "-e") == 0)
                {
                    plotOpts.push_back('e');
                    values.push_back(atof(argv[i + 1]));
                    ++nPlots;
                }
                else if(strcmp(argv[i], "-b") == 0)
                {
                    plotOpts.push_back('b');
                    values.push_back(atof(argv[i + 1]));
                    ++nPlots;
                }
                else if(strcmp(argv[i], "-f") == 0)
                {
                    extention = argv[i + 1];
                }
                else if(strcmp(argv[i], "--p") == 0)
                {
                    print = true;
                    --i;
                }
                else
                {
                    PrintUsage();
                    return 0;
                }
            }
        }
        catch(std::exception const &e)
        {
            std::cerr << "Error : invaild input argument." << std::endl;
            std::cerr << "what() : " << e.what() << std::endl;
            return -1;
        }
    }
    MediumMagboltz *gas = new MediumMagboltz();
    if(!gas->LoadGasFile(gasFileName))
    {
        cerr << "Failed to load gas file : " + gasFileName << endl;
        return -1;
    }
    vector<double> eField, bField, angle;
    gas->GetFieldGrid(eField, bField, angle);
    double eMin = eField.front(), eMax = eField.back();
    double bMin = bField.front(), bMax = bField.back();
    std::cout << "Start plotting..." << std::endl;
    TApplication app("app", &argc, argv);
    ViewMedium mediumView;
    mediumView.SetMedium(gas);
    mediumView.SetRangeE(eMin, eMax, false);
    mediumView.SetRangeB(bMin, bMax, false);
    for(int i = 0;i < nPlots;++i)
    {
        if(plotOpts[i] == 'e')
            mediumView.SetMagneticField(values[i]);
        else if(plotOpts[i] == 'b')
            mediumView.SetElectricField(values[i]);
        else
        {
            std::cout << "Invalid flag of physical quantity for x-axis." << std::endl;
            continue;
        }
        TCanvas *c1 = new TCanvas();
        mediumView.SetCanvas(c1);
        mediumView.PlotElectronDiffusion(plotOpts[i]);
        TCanvas *c2 = new TCanvas();
        mediumView.SetCanvas(c2);
        mediumView.PlotElectronVelocity(plotOpts[i]);
        if(print)
        {
            std::string file = gasFileName.substr(0, gasFileName.find_last_of('.') - 1);
            std::string outFileName;
            char buffer[256];
            if(plotOpts[i] == 'e')
                sprintf(buffer, "%s_B%1.2f", file.c_str(), values[i]);
            else if(plotOpts[i] == 'b')
                sprintf(buffer, "%s_E%03d", file.c_str(), (int)values[i]);
            outFileName = std::string(buffer) + "_diff." + extention;
            c1->Print(outFileName.c_str(), extention.c_str());
            outFileName = std::string(buffer) + "_vel." + extention;
            c2->Print(outFileName.c_str(), extention.c_str());
        }
    }
    std::cout << "Plotting done..." << std::endl;
    app.Run(false);
    return 0;
}