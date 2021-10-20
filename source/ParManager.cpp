#include "ParManager.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <cstdlib>

ParManager* ParManager::fInstance = nullptr;
string ParManager::fFileName = "";
unordered_map<string, string> ParManager::fParMap = std::unordered_map<string, string>();

ParManager* ParManager::getInstance(string fileName)
{
    if(fInstance == nullptr)
        fInstance = new ParManager(fileName);
    return fInstance;
}

void ParManager::initPars()
{
    int lineNum = 0;
    vector<string> pars;
    std::string lineStr = "";
    std::string command = "";    
    ifstream fIn(fFileName.data(), ios::in);
    cout << "File " << fFileName << " opened." << endl;
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
            printf("In ParManager::initPars() :\n" );
            printf("Error : reading line %4d in file %s.\n", lineNum, lineStr.data());
            continue;
        }
        else
            fParMap[pars[0]] = pars[1];
    }
    cout << fParMap.size() << " parameters registered." << endl;
    fIn.close();
    cout << "File " << fFileName << " closed." << endl;

}

double ParManager::getParD(string parName) const
{
    auto it = fParMap.find(parName);
    if(it == fParMap.end())
    {
        cout << "In ParManager::getParD(string parName)." << endl;
        cout << "Error : Cannot find parameter " << parName << "." << endl;
        return 0.;
    }
    return atof(it->second.data());
}


std::string ParManager::getParS(string parName) const
{
    auto it = fParMap.find(parName);
    if(it == fParMap.end())
    {
        cout << "In ParManager::getParS(string parName)." << endl;
        cout << "Error : Cannot find parameter " << parName << "." << endl;
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

bool ParManager::isEmptyLine(string str)
{
    for(auto i : str)
        if(!isspace(i))
            return false;
        return true;        
}

vector<string> ParManager::splitString(string str, char delim)
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

