#ifndef SRIMTRACKBUILDER_HPP
#define SRIMTRACKBUILDER_HPP

#include "Garfield/TrackSrim.hh"
#include "Garfield/Sensor.hh"
#include "Garfield/MediumMagboltz.hh"

#include <string>
#include <memory>

// this builder class builds Garfield::TrackSrim instance.
class SrimTrackBuilder {
    public:
    SrimTrackBuilder();
    SrimTrackBuilder(const std::string &srimFile);

    virtual ~SrimTrackBuilder() {}

    // Build a TrackSrim instance and return its pointer.
    // Desruction of an instance is an users reponsibility.
    std::unique_ptr<Garfield::TrackSrim> build(Garfield::Sensor *sensor = nullptr);

    void setPressure(int pressure);

    void setComposition(
        const std::string gas1, int frac1 = 100,
        const std::string gas2 = "", int frac2 = 0,
        const std::string gas3 = "", int frac3 = 0,
        const std::string gas4 = "", int frac4 = 0);
    void setComposition(
        const std::string gas1, double frac1 = 1.,
        const std::string gas2 = "", double frac2 = 0.,
        const std::string gas3 = "", double frac3 = 0.,
        const std::string gas4 = "", double frac4 = 0.);
    void reset();
    void print();
    void setModel(const int model) { fModel = model; }
    void setSrimFileName(const std::string srimFile) { fSrimFile = srimFile; }


    bool gasIntialized() const;

    private:

    private:
    std::unique_ptr<Garfield::MediumMagboltz> gas;
    std::string fSrimFile;
    int fModel;
};
#endif // SRIMTRACKBUILDER_HPP