# this python script reads parameter files and generates scripts a tripple GEM geometry(.geo) and start information file(.sif)
#!/usr/bin/env python
import os
import sys

absPath = os.path.realpath(__file__)
absPath = absPath[:absPath.rfind("/")]
absPath = absPath[:absPath.rfind("/")]
sys.path.append(absPath + "/pysource/")
import parManager as pr

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

if not os.path.exists("./" + scriptName + "/"):
    os.mkdir("./" + scriptName + "/")

print("---------------------------------------------------------------------------------------------")
if "-geo" in sys.argv or "-all" in sys.argv:
    # generating geometry(.geo) file
    print("Generating " + scriptName + ".geo file..., ", end="")
    geoTemplate = open(absPath + "/templates/geometry_template.txt")
    geo = reader.insertParValuesInText("".join(geoTemplate.readlines()))
    geoTemplate.close()
    geoFile = open(scriptName + ".geo", "w")
    geoFile.writelines(geo)
    geoFile.close()
    print("done.")
if "-sif" in sys.argv or "-all" in sys.argv:
    # generating start information file(.sif) file for ElmerSolve
    print("Generating " + scriptName + ".sif file..., ", end="")
    sifTemplate = open(absPath + "/templates/startinfo_template.txt")
    sif = reader.insertParValuesInText("".join(sifTemplate.readlines()))
    sifTemplate.close()
    sifFile = open(scriptName + ".sif", "w")
    sifFile.writelines(sif)
    sifFile.close()
    print("done.")
if "-diel" in sys.argv or "-all" in sys.argv:
    # generating ./[SCRIPT_NAME]/dielectrics.dat file for ElmerSolve
    print("Generating " + scriptName + "/dielectrics.dat file..., ", end="")
    dielTemplate = open(absPath + "/templates/dielectrics_template.txt")
    diel = reader.insertParValuesInText("".join(dielTemplate.readlines()))
    dielFile = open(scriptName + "/dielectrics.dat", "w")
    dielFile.writelines(diel)
    dielFile.close()
    print("done.")
print("---------------------------------------------------------------------------------------------")
