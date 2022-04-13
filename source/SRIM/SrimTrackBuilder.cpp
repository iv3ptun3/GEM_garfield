#include <iostream>

#include "SRIM/SrimTrackBuilder.hpp"
#include "Exception.hpp"

using namespace Garfield;
using namespace std;

SrimTrackBuilder::SrimTrackBuilder()
    : gas(new MediumMagboltz), fSrimFile(""), fModel(4)
{
}

SrimTrackBuilder::SrimTrackBuilder(const string &srimFile)
    : SrimTrackBuilder()
{
    fSrimFile = srimFile;
}

void SrimTrackBuilder::setPressure(int pressure)
{
    gas->SetPressure((double)pressure);
}

void SrimTrackBuilder::setComposition(
    const string gas1, int frac1,
    const string gas2, int frac2,
    const string gas3, int frac3,
    const string gas4, int frac4)
{
    gas->SetComposition(gas1, (double)frac1, gas2, (double)frac2, gas3, (double)frac3, gas4, (double)frac4);
}

void SrimTrackBuilder::setComposition(
    const string gas1, double frac1,
    const string gas2, double frac2,
    const string gas3, double frac3,
    const string gas4, double frac4)
{
    if(gas->SetComposition(gas1, frac1, gas2, frac2, gas3, frac3, gas4, frac4))
        throw Exception("Failed to initialzie a Magboltz instance to evaluate gas properties.");
}

void SrimTrackBuilder::reset()
{
    gas = make_unique<MediumMagboltz>();
}

void SrimTrackBuilder::print()
{
    gas->PrintGas();
}

std::unique_ptr<Garfield::TrackSrim> SrimTrackBuilder::build(Sensor *sensor)
{
    std::unique_ptr<Garfield::TrackSrim>track(new TrackSrim);
    if(!track->ReadFile(fSrimFile))
        throw Exception("Failed to load SRIM data file : " + fSrimFile);
    else if(!gasIntialized())
        throw Exception("Gas composition is not set.");

    track->SetAtomicMassNumbers(gas->GetAtomicWeight(), gas->GetAtomicNumber());
    track->SetFanoFactor(gas->GetFanoFactor());
    track->SetWorkFunction(gas->GetW());
    track->SetModel(fModel);
    if(sensor != nullptr)
        track->SetSensor(sensor);
    return move(track);
}

bool SrimTrackBuilder::gasIntialized() const
{
    return gas->IsGas() && gas->IsIonisable() && gas->IsDriftable();
}