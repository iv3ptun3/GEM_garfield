#include <fstream>
#include <sstream>
#include <iostream>
#include <cstdlib>

#include "Errors.hpp"
#include "ParManager.hpp"

ParManager* ParManager::fInstance = nullptr;
unordered_map<string, string> ParManager::fParMap = std::unordered_map<string, string>();

ParManager::~ParManager()
{
    fInstance = nullptr;
    fParMap.clear();
    cout << "Fuck" << endl; 
}

ParManager* ParManager::getInstance()
{
    if(fInstance == nullptr)
        fInstance = new ParManager();
    return fInstance;
}

void ParManager::initPars(const string fileName)
{
    int lineNum = 0;
    vector<string> pars;
    std::string lineStr = "";
    std::string command = "";    
    ifstream fIn(fileName.data(), ios::in);
    cout << "File " << fileName << " opened." << endl;
    while(true)
    {
        // check end of the line
        if(fIn.eof())
            break;
        getline(fIn, lineStr);
        ++lineNum;
        command = lineStr.substr(0, lineStr.find('#'));
        pars = splitString(command, ' ');
        // check empty line
        if(isEmptyLine(command))
            continue;
        else if(pars.size() != 2)
        {
            std::string message = "Cannot interpret the line " + std::to_string(lineNum) + ".";
            printError("ParManager", "initPars(string)", message);
            continue;
        }
        else
            fParMap[pars[0]] = pars[1];
    }
    cout << fParMap.size() << " parameters registered." << endl;
    fIn.close();
    cout << "File " << fileName << " closed." << endl;

}

double ParManager::getParD(const string parName) const
{
    auto it = fParMap.find(parName);
    if(it == fParMap.end())
    {
        std::string message = "A parameter " + parName + " is not registered.";
        printError("ParManager", "getParD(const string)", message);
        return 0.;
    }
    return atof(it->second.data());
}


std::string ParManager::getParS(const string parName) const
{
    auto it = fParMap.find(parName);
    if(it == fParMap.end())
    {
        std::string message = "A parameter " + parName + " is not registered.";
        printError("ParManager", "getParS(const string)", message);
        return "";
    }
    return it->second;
}

void ParManager::listPars() const
{
    cout << "Listing " << fParMap.size() << " parameters..." << endl;
    for(auto i : fParMap)
    {
        printf("%-30s %s\n", i.first.data(), i.second.data());
    }
}

bool ParManager::isEmptyLine(const string str)
{
    for(auto i : str)
        if(!isspace(i))
            return false;
        return true;        
}

vector<string> ParManager::splitString(const string str, char delim)
{
    string tmp;
    vector<string> strVec;
    stringstream ss(str);
    while(getline(ss, tmp, delim))
    {
        if(tmp.length() > 0)
            strVec.emplace_back(tmp);
    }
    return strVec;
}

