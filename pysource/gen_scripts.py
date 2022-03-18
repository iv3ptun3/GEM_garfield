# this python script reads parameter files and generates scripts a tripple GEM geometry(.geo) and start information file(.sif)
#!/usr/bin/env python
import os
import sys
sys.path.append("../scripts/")
import parManager as pr
import geometry as tgg
import startinfo as tgs
import dielectrics as dls

if len(sys.argv) < 3 or "--help" in sys.argv:
    print("Usage : python3 gen_fm [parameter file name] [option -geo, -sif, -diel, -all]")
    print("-geo : generate .geo file.")
    print("-sif : generate .sif file.")
    print("-diel : generate ./[SCRIPT_NAME]/dielectrics.dat file.")
    print("-all : generate all scripts.")
    exit()

# loading parameters
reader = pr.ParManager.instance(True)
reader.initPars(sys.argv[1])
scriptName = reader.getPar("SCRIPT_NAME")
print("---------------------------------------------------------------------------------------------")
if "-geo" in sys.argv or "-all" in sys.argv:
    # generating geometry(.geo) file
    print("Generating " + scriptName + ".geo file..., ", end="")
    geo = reader.insertParValuesInText(tgg.geometry_txt)
    geoFile = open(scriptName + ".geo", "w")
    geoFile.writelines(geo)
    geoFile.close()
    print("done.")
if "-sif" in sys.argv or "-all" in sys.argv:
    # generating start information file(.sif) file for ElmerSolve
    print("Generating " + scriptName + ".sif file..., ", end="")
    sif = reader.insertParValuesInText(tgs.startinfo_txt)
    sifFile = open(scriptName + ".sif", "w")
    sifFile.writelines(sif)
    sifFile.close()
    print("done.")
if "-diel" in sys.argv or "-all" in sys.argv:
    # generating ./[SCRIPT_NAME]/dielectrics.dat file for ElmerSolve
    print("Generating " + scriptName + "/dielectrics.dat file..., ", end="")
    diel = reader.insertParValuesInText(dls.dielectrics_txt)
    if not os.path.exists("./" + scriptName + "/"):
        os.mkdir("./" + scriptName + "/")
    dielFile = open(scriptName + "/dielectrics.dat", "w")
    dielFile.writelines(diel)
    dielFile.close()
    print("done.")
print("---------------------------------------------------------------------------------------------")
