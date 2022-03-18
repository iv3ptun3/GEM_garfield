# this python script generates field map files by running Gmsh and Elmer.
# and run all commands to generate mesh and field map data.
#!/usr/bin/env python
import os
import sys
sys.path.append("../scripts/")
import parManager as pr

if len(sys.argv) != 2 or "--help" in sys.argv:
    print("Usage : python3 gen_fieldmap [parameter file name]")
    exit()

# loading parameters
reader = pr.ParManager.instance(False)
reader.initPars(sys.argv[1])
scriptName = reader.getPar("SCRIPT_NAME")

path = "./" + scriptName
if not os.path.exists(path):    
    os.system("mkdir " + scriptName)

# running commands
print("---------------------------------------------------------------------------------------------")
print("3D Meshing geometry file using Gmsh..., ", end="")
os.system("gmsh " + scriptName + ".geo -3 -order 2 >> " + scriptName + "/log_geo.txt")
print("Done. Logs are saved as " + scriptName + "/log_geo.txt file")
print("Griding mesh file using ElmerGrid..., ", end="")
os.system("ElmerGrid 14 2 " + scriptName + ".msh -autoclean >> " + scriptName + "/log_grd.txt")
print("Done. Logs are saved as " + scriptName + "/log_grd.txt file")
print("Solving field equation using ElmerSolver..., ", end="")
os.system("ElmerSolver " + scriptName + ".sif >> " + scriptName + "/log_sol.txt")
print("Done. Logs are saved as " + scriptName + "/log_sol.txt file")
print("---------------------------------------------------------------------------------------------")
