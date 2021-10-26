Written by Hyunmin Yang, HANUL, Korea University.

This program simulates AT-TPC(Active Target Time Projection Chamber) with tripple GEMs.
Interation between charged particles and gas medium will be simulated with Geant4.
Drift and multiplication of electrons ionized by charged particle will be simulated with Garfield++.

# Environment
Ubuntu or MaxOs(maybe?).

# Prerequisites
ROOT version 6  
Garfield++  
Geant4  
Gmsh  
Elmer

# Executables
gen_scripts.py          : generates scripts(.geo, .sif and dielectris.dat) file reading a parameter file.  
gen_fieldmap.py         : Run Gmsh and Elmer programs with the scripts to create field map data files.  
view_tgem               : Plot equipotential lines and drift lines of tripple GEMs.

# Garfield++
All physical information(electric field, GEM voltages and geometry) is managed by parManager class(both c++ and python).
If you want to modify it, you must write a text file including all paramter lists(see **params.txt**).
## Generating scripts
First of all, you need to generate script files as input for Gmsh and ElmerGrid, Elmersolver.
By running script **gen_scripts.py**, parameters listed in a parameter file will be inserted into script templets **geometry.py**, **startinfo.py** and **dielectrics.py**.
```
mkdir build
cd build
python3 gen_scripts.py [parameter file name] -all
```
First input is a parameter file containing full information of this project.
For example, SCRIPT_NAME is defined in the first line of **params.txt**.
After running, **SCRIPT_NAME.geo**, **SCRIPT_NAME.sif** and **SCRIPT_NAME/dielectrics.dat** will be created on your directory.
About the format of parameter file, see **params.txt**
## Meshing
Next, you have to generate a 3D mesh file describing unit cell of tripple GEM.
```
gmsh SCRIPT_NAME.geo -3 -order 2
```
After running, **SCRIPT_NAME.msh** will be created on your directory.
## Generating field map data.
```
ElmerGrid 14 2 SCRIPT_NAME.msh -autoclean
ElmerSolver [SCRIPT_NAME].sif
```
Running ElmerGrid converts .msh file to **mesh.boundary**, **mesh.elements**, **mesh.header** and **mesh.nodes** in the SCRIPT_NAME diretory. The second command solves the field equation with **SCRIPT_NAME.sif**(start information file).
**SCRIPT_NAME.ep** and **SCRIPT_NAME.result** will be created the SCRIPT_NAME directory.

Or you can go through all with one script.
```
python3 gen_fieldmap.py [parameter file name]
```
## Importing field map data to Garfield++
Following all steps above, required files are ready in the SCRIPT_NAME directory.
1. **SCRIPT_NAME/dielectrics.dat** : First line tells the number of material(different volumes with same material are considered to have different material ID.) and the list of material IDs and its relative permitivity.
2. **mesh.boundary**, **mesh.elements**, **mesh.header** and **mesh.nodes** : geometry information.
3. **SCRIPT_NAME/SCRIPT_NAME.result** : field map solution.