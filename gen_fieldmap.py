# this python script generates field map files by running Gmsh and Elmer.
# and run all commands to generate mesh and field map data.
#!/usr/bin/env python
import os
import sys
sys.path.append("./pysource/")

if len(sys.argv) != 1 or "--help" in sys.argv:
    print("Usage : python3 gen_fieldmap")
    exit()

print("3D Meshing geometry file using Gmsh..., ", end="")
os.system("gmsh tgemcell.geo -3 -order 2 >> tgemcell/log_geo.txt")
print("Done. Logs are saved as tgemcell/log_geo.txt file")
print("Griding mesh file using ElmerGrid..., ", end="")
os.system("ElmerGrid 14 2 tgemcell.msh -autoclean >> tgemcell/log_grd.txt")
print("Done. Logs are saved as tgemcell/log_grd.txt file")
print("Solving field equation using ElmerSolver..., ", end="")
os.system("ElmerSolver tgemcell.sif >> tgemcell/log_sol.txt")
print("Done. Logs are saved as tgemcell/log_sol.txt file")
