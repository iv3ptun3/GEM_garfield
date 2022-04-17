#include <time.h>

#include "Garfield/MediumMagboltz.hh"
#include "Garfield/Sensor.hh"
#include "Garfield/AvalancheMicroscopic.hh"
#include "Garfield/AvalancheMC.hh"

#include "ComponentFactory.hpp"
#include "MediumMagboltzFactory.hpp"

#include "ParManager.hpp"

#include "analyzer/RunAnalyzer.hpp"
#include "analyzer/GainEventAnalyzer.hpp"

using namespace Garfield;
using namespace std;

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        cout << "Usage : tgem_gain [parameter file] [nEvent]" << endl;
        return 0;
    }
    const string parFileName = argv[1];
    const unsigned int nEvt = atoi(argv[2]);

    // Initializing a global manager for configure parameters.
    auto parMan = ParManager::getInstance();
    parMan->initPars(parFileName);     
    // tripple gem dimension
    const double pitch = parMan->getParD("PITCH");
    const double tD = parMan->getParD("T_DIEL");
    const double dZp = parMan->getParD("DZ_PADPLANE");
    const double dZ12 = parMan->getParD("DZ_GEM12");
    const double dZ23 = parMan->getParD("DZ_GEM23");
    const double dZe = parMan->getParD("DZ_ELECTRODE");
    const double dZu = parMan->getParD("DZ_UPPERPLANE");
    const double tpcX = parMan->getParD("TPC_X");
    const double tpcY = parMan->getParD("TPC_Y");
    // voltage
    const double dvg1 = parMan->getParD("DV_GEM1");
    const double dvg2 = parMan->getParD("DV_GEM2");
    const double dvg3 = parMan->getParD("DV_GEM3");
    // electric field density
    const double eTrans = parMan->getParD("E_TRANS");
    const double eDrift = parMan->getParD("E_DRIFT");
    const double eInduction = parMan->getParD("E_INDUCTION");
    // magnetic field
    const double bField = parMan->getParD("B_Z");
    // elmer data name
    const string scriptDir = parMan->getParS("SCRIPT_NAME");
    const string gasFileName = parMan->getParS("GAS_FILE");
    
    unique_ptr<MediumMagboltz> gas = MediumMagboltzFactory::createFromGasFile(gasFileName);

    unique_ptr<ComponentElmer> componentGem = ComponentFactory::createElmer(gas.get(), scriptDir);
    unique_ptr<ComponentConstant> componentConst = ComponentFactory::createConstB(gas.get(), bField
        , -tpcX/2, -tpcY/2, 0, tpcX/2, tpcY/2, dZp + dZ12 + dZ23 + dZu);

    // Create the sensor.
    Sensor sensor;
    sensor.AddComponent(componentGem.get());
    if(bField > 0.0)
		sensor.AddComponent(componentConst.get());
    sensor.SetArea(-tpcX/2, -tpcY/2, 0, tpcX/2, tpcY/2, dZp + dZ12 + dZ23 + dZu);

    AvalancheMicroscopic aval;
    aval.SetSensor(&sensor);
    aval.SetTimeWindow(0., 10e3);

    AvalancheMC drift;
    drift.SetSensor(&sensor);
    drift.SetDistanceSteps(2.e-4);
    drift.SetTimeWindow(0., 10e3); // only calculate to 10 us.

    // analyzers
    RunAnalyzer runAna;
    GainEventAnalyzer eventAna(&aval);
    runAna.Open(scriptDir + "/out.root");
    runAna.SetEventAnalyzer(&eventAna);
    runAna.SaveByEvent();
    // event loop
    const int timeOut = 216000; // 2.5 days
    cout << "Begin of Event Loop..." << endl;
    for (long long evtId = 0; evtId < nEvt; ++evtId)
    {
        if((int)clock()/CLOCKS_PER_SEC > timeOut)
        {
            cout << "Time out, escaping for loop..." << endl;
            break;
        }
        aval.AvalancheElectron(
            0., 0., dZp + dZ12 + dZ23 + dZu/20.,
            0., 0., 0., 0., 0.);
        runAna.ProcessEvent();
    }
    runAna.Write();
    runAna.Close();
    // delete allocated memory
    delete parMan;
    return 0;
}
