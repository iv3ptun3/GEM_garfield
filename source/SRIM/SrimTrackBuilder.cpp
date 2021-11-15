#include "SrimTrackBuilder.hpp"
#include "Errors.hpp"

SrimTrackBuilder::SrimTrackBuilder()
    : fSrimFile(""), fEffAtomicMass(0.), fEffAtomicNum(0.), fEffFanoFactor(0.), fEffWorkFunction(0.), fModel(4), fNumberOfCompounds(0)
{
}

SrimTrackBuilder::SrimTrackBuilder(const string &srimFile)
    : SrimTrackBuilder()
{
    fSrimFile = srimFile;
}

SrimTrackBuilder *SrimTrackBuilder::addCompound(const string &name, const double frac, const double A, const double Z, const double fano, const double work)
{
    ++fNumberOfCompounds;
    fNames.emplace_back(name);
    fFractions.push_back(frac);
    fAtomicMasses.push_back(A);
    fAtomicNums.push_back(Z);
    fFanoFactors.push_back(fano);
    fWorkFuncions.push_back(work);
    return this;
}

void SrimTrackBuilder::clearCompounds()
{
    fNumberOfCompounds = 0;
    fNames.clear();
    fFractions.clear();
    fAtomicMasses.clear();
    fAtomicNums.clear();
    fFanoFactors.clear();
    fWorkFuncions.clear();
}

void SrimTrackBuilder::print()
{
    std::cout << "SrimTrackBuilder : Medium consists of " << fNumberOfCompounds << " compounds(elements).\n";
    if(fNumberOfCompounds > 0)
    {
        std::cout << "--------------------------------------------------------------------------------------------------\n";
        std::cout << "      Name  " << "    Fraction(%)     " <<
            "       A   " << "      Z   " << "      Fano Factor     " << "  Work Function(eV)   \n";
        for(int i = 0;i < fNumberOfCompounds;++i)
        {
            printf("%12s", fNames.at(i).c_str());
            printf("%18.2f", fFractions.at(i));
            printf("%10.2f", fAtomicMasses.at(i));
            printf("%10.2f", fAtomicNums.at(i));
            printf("%20.2f", fFanoFactors.at(i));
            printf("%21.2f\n", fWorkFuncions.at(i));
        }
        update();
        printf("Efficient Atomic Mass       : %.3f\n", fEffAtomicMass);
        printf("Efficient Atomic Num        : %.3f\n", fEffAtomicNum);
        printf("Efficient Fano Factor       : %.3f\n", fEffFanoFactor);
        printf("Efficient Work Function(eV) : %.3f\n", fEffWorkFunction);
        std::cout << "--------------------------------------------------------------------------------------------------\n";
    }
}

TrackSrim *SrimTrackBuilder::build(Sensor *sensor)
{
    // check the sum of fractions of gas mixture
    double sum = 0;
    for(auto f : fFractions)
        sum += f;
    if((sum - 100.)*(sum - 100.)/10000 > kFracSumTol)
    {
        printError("SrimTrackBuilder", "build()",
            "The sum of fractions(" + std::to_string(sum) + " %%) is not close to 100 %%.");
    }
    TrackSrim *track = new TrackSrim();
    if(!track->ReadFile(fSrimFile))
    {
        printError("SrimTrackBuilder", "build()",
            "Failed to import SRIM data file " + fSrimFile + ".");
    }
    // update efficient parameters
    update();
    track->SetAtomicMassNumbers(fEffAtomicMass, fEffAtomicNum);
    track->SetFanoFactor(fEffFanoFactor);
    track->SetWorkFunction(fEffWorkFunction);
    track->SetModel(fModel);
    if(sensor != nullptr)
        track->SetSensor(sensor);
    return track;
}

void SrimTrackBuilder::update()
{
    fEffAtomicNum = weightedAvarage(fAtomicNums, fFractions);
    fEffAtomicMass = weightedAvarage(fAtomicMasses, fFractions);
    fEffFanoFactor = weightedAvarage(fFanoFactors, fFractions);
    fEffWorkFunction = weightedAvarage(fWorkFuncions, fFractions);
}

const double SrimTrackBuilder::weightedAvarage(const vector<double> &vec, const vector<double> &w) const
{
    double sum = 0, wSum = 0;
    for(int i = 0;i < vec.size();++i)
    {
        sum += vec.at(i)*w.at(i);
        wSum += w.at(i);
    }
    return sum/wSum;
}