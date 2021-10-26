#ifndef PARMANGER_HPP
#define PARMANGER_HPP

// this class load parameter file.
#include <string>
#include <unordered_map>
#include <vector>
using namespace std;

class ParManager{
    public:
        static ParManager* getInstance();
        static void initPars(const string fileName);
        double getParD(const string parName) const;
        std::string getParS(const string parName) const;
        void listPars() const;
    private:
        ParManager(){}
        static bool isEmptyLine(const string str);
        // split string separated by multiple spaces
        static vector<string> splitString(const string str, char delim);
    private:
        static ParManager* fInstance;
        static unordered_map<string, string> fParMap;
};

#endif // PARMANGER_HPP