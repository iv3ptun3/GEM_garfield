#ifndef SRIMTRACKBUILDER_HPP
#define SRIMTRACKBUILDER_HPP

#include "Garfield/TrackSrim.hh"
#include "Garfield/Sensor.hh"

#include <string>

using namespace Garfield;
using namespace std;

// this builder class builds Garfield::TrackSrim instance.
class SrimTrackBuilder{
    public:
        SrimTrackBuilder();
        SrimTrackBuilder(const string &srimFile);
        
        virtual ~SrimTrackBuilder(){}

        SrimTrackBuilder* addCompound(const string &name, const double frac, const double A, const double Z,  const double fano, const double work);
        void clearCompounds();
        
        void print();

        void setModel(const int model){fModel = model;}
        // Build a TrackSrim instance and return its pointer.
        // Desruction of an instance is an users reponsibility.
        TrackSrim* build(Sensor *sensor = nullptr);
        
    private:
        // update all fields by calculating efficient constant.
        void update();
        const double weightedAvarage(const vector<double> &vec, const vector<double> &w) const;
    private:
        string fSrimFile;
        double fEffAtomicMass, fEffAtomicNum, fEffFanoFactor, fEffWorkFunction;
        int fModel;
        int fNumberOfCompounds;
        vector<string> fNames;
        vector<double> fFractions, fAtomicMasses, fAtomicNums, fFanoFactors, fWorkFuncions;
        // to check the sum of fractions to be 100 %
        static constexpr double kFracSumTol = 1e-3;
};

#endif // SRIMTRACKBUILDER_HPP