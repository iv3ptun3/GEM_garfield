# this python script reads parameter files and generates scripts a tripple GEM geometry(.geo) and start information file(.sif)
#!/usr/bin/env python
import os
import sys
sys.path.append("./pysource/")
import parManager as pr
import tgemcell_geo as tgg
import tgemcell_sif as tgs

if len(sys.argv) < 3 or "--help" in sys.argv:
    print("Usage : python3 gen_fm [parameter file name] [option -geo, -sif, -all]")
    print("-geo : generate .geo file.")
    print("-sif : generate .sif file.")
    print("-all : generate all scripts.")
    exit()

# loading parameters
reader = pr.ParManager.instance(True)
reader.initPars("params.txt")
if "-geo" in sys.argv or "-all" in sys.argv:
    # generating geometry(.geo) file
    print("Generating tgemcell.geo file..., ", end="")
    geo = reader.insertParValuesInText(tgg.tgemcell_geo)
    geoFile = open("tgemcell.geo", "w")
    geoFile.writelines(geo)
    geoFile.close()
    print("done.")
if "-sif" in sys.argv or "-all" in sys.argv:
    # generating start information file(.sif) file for ElmerSolve
    print("Generating tgemcell.sif file..., ", end="")
    sif = reader.insertParValuesInText(tgs.tgemcell_sif)
    sifFile = open("tgemcell.sif", "w")
    sifFile.writelines(sif)
    sifFile.close()
    print("done.")
