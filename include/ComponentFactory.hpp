#ifndef COMPONENTFACTORY_HPP
#define COMPONENTFACTORY_HPP

#include "Garfield/ComponentConstant.hh"
#include "Garfield/ComponentElmer.hh"
#include "Garfield/MediumMagboltz.hh"

#include <string>
#include <exception>
#include <memory>

class ComponentFactory {
    public:
    ComponentFactory();
    virtual ~ComponentFactory();
    static std::unique_ptr<Garfield::ComponentElmer> createElmer(Garfield::MediumMagboltz *medium, const std::string &name);
    static std::unique_ptr<Garfield::ComponentConstant> createConstE(Garfield::MediumMagboltz *medium, double Ez,
        double x0, double y0, double z0,
        double dx, double dy, double dz);
    static std::unique_ptr<Garfield::ComponentConstant> createConstB(Garfield::MediumMagboltz *medium, double Bz,
        double x0, double y0, double z0,
        double dx, double dy, double dz);
    private:
    static const std::string getExeAbsPath();
};

class ComponentFactoryException : public std::exception{
    public:
    ComponentFactoryException(const std::string &_message)
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

#endif // COMPONENTFACTORY_HPP
