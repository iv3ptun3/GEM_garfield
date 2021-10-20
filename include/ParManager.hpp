#ifndef PARMANGER_HPP
#define PARMANGER_HPP

// this class load parameter file.
#include <string>
#include <unordered_map>
#include <vector>
using namespace std;

class ParManager{
    public:
        ParManager() = delete;
        static ParManager* getInstance(string fileName);
        static void initPars();
        double getParD(string parName) const;
        std::string getParS(string parName) const;
        void listPars() const;
    private:
        ParManager(string fileName){fFileName = fileName;}
        static bool isEmptyLine(string str);
        // split string separated by multiple spaces
        static vector<string> splitString(string str, char delim);
    private:
        static string fFileName;
        static ParManager* fInstance;
        static unordered_map<string, string> fParMap;
};

#endif // PARMANGER_HPP