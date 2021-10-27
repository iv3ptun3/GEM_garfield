#ifndef ERRORS_HPP
#define ERRORS_HPP

#include <iostream>
#include <string>
static void printError(
    const std::string cls, const std::string method, const std::string message)
{
    std::cerr << "-------------------------------------------------------------------" << std::endl;
    std::cerr << "Error in " << cls << "::" << method << "," << std::endl;
    std::cerr << "\t" << message << std::endl;
    std::cerr << "-------------------------------------------------------------------" << std::endl;
}

#endif // ERRORS_HPP