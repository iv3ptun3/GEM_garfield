#ifndef MEDIUMMAGBOLTZFACTORY_HPP
#define MEDIUMMAGBOLTZFACTORY_HPP

#include "Garfield/MediumMagboltz.hh"

#include <string>
#include <exception>
#include <memory>
#include <unordered_map>

// A factory pattern creating prototype instances.
class MediumMagboltzFactory {
    public:
    static std::unique_ptr<Garfield::MediumMagboltz> createGasMixture(
        const std::string &mediumn, double fracMedium,
        const std::string &quenching, double fracQuenching);

    private:
    static bool checkFraction(double frac1, double frac2);
    static bool ionMobilityDirExists(const std::string &ionName);
    static std::string getIonMobilityDir(const std::string &ionName);

    static std::unordered_map<std::string, std::string> ionMobilityDirs;
};

class MediumMagboltzFactoryException : public std::exception {
    public:
    MediumMagboltzFactoryException(const std::string &_message)
        : std::exception(), message(_message)
    {
    }
    const char *what() const noexcept override
    {
        return message.c_str();
    }
    private:
    const std::string message;
};

#endif // MEDIUMMAGBOLTZFACTORY_HPP
