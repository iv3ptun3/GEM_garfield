#include "TTree.h"
#include "TFile.h"
#include <stdlib.h>
#include <iostream>
// this program merges trees created via condor_run.sh.
int main(int argc, char *argv[])
{
    if(argc != 5)
    {
        printf("Usage : ./merge_tree [outfile] [subdirectory name] [tree name] [number of threads]\n");
        printf("if outfile is [directory]/out.root and the subdirectory name is tgem,\n");
        printf("This program search for [directory]/tgem0000/out0000.root, [directory]/tgem0001/out0001.root ... and merge trees saved in them.\n");
        return 0;
    }
    const std::string outfile = argv[1];
    std::string path = outfile.substr(0, outfile.find_last_of('/') + 1);
    const std::string subdir = argv[2];
    const char *treeName = argv[3];
    const unsigned int nThreads = atoi(argv[4]);
    TFile *fileOut = TFile::Open(outfile.c_str(), "RECREATE");
    TFile *fileIn;
    std::string treeFile;
    char buffer[256];
    std::vector<TTree*> trees(nThreads, nullptr);
    TList* list = new TList();
    for(int t = 0;t < nThreads;++t)
    {
        sprintf(buffer, "%s_%04u/%s.root", subdir.c_str(), t, subdir.c_str());
        treeFile = path + buffer;
        fileIn = TFile::Open(treeFile.c_str(), "READ");
        trees.at(t) = (TTree*)fileIn->Get(treeName);
        if(trees.at(t) == nullptr)
        {
            printf("Error, cannot find tree %s in %s\n", treeName, treeFile.c_str());
            return -1;
        }
        list->AddLast(trees.at(t));
        std::cout << "Found tree " << treeName << " in the root file " << treeFile << "...\n";
    }
    fileOut->cd();
    TTree *tree = TTree::MergeTrees(list);    
    tree->SetNameTitle(treeName, treeName);
    tree->Write();
    fileOut->Close();
    printf("All trees merge to %s\n", outfile.c_str());
    return 0;
}