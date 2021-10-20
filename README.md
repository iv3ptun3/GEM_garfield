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
gen_scripts.py          : generates scripts(.geo and .sif) file reading a parameter file.  
gen_fieldmap.py         : Run Gmsh and Elmer programs with the scripts to create field map data files.  
view_tgem               : Draw equipotential lines and drift lines of tripple GEMs.

# Garfield++
All physical information(electric field, GEM voltages and geometry) is managed by parManager class(both c++ and python).
If you want to modify it, you must write a text file including all paramter lists(see params.txt).
## Generating scripts
First of all, you need to generate script files as input for Gmsh and ElmerGrid, Elmersolver.
By running script **gen_scripts.py**, parameters listed in a parameter file will be inserted into script templets **tgemcell_geo.py** and **tgemcell_sif.py**.
```
mkdir build
cd build
python3 gen_scripts.py [parameter file name] -all
```
After running, **tgemcell.geo** and **tgemcell.sif** will be created on your directory.
About the format of parameter file, see **params.txt**
## Meshing
Next, you have to generate a 3D mesh file describing unit cell of tripple GEM.
```
gmsh tgemcell.geo -3 -order 2
```
After running, **tgemcell.msh** will be created on your directory.
## Generating field map data.
```
ElmerGrid 14 2 tgemcell.msh -autoclean
ElmerSolver tgemcell.sif
```
Run ElmerGrid to convert .msh file to **mesh.boundary**, **mesh.elements**, **mesh.header** and **mesh.nodes**. Next, solve the field equation with **tgemcell.sif**(start information file).
**tgemcell.ep** and **tgemcell.result** will be reated on your directory.

Or you can go through all with one script.
```
python3 gen_fieldmap.py
```
## Importing field map data to Garfield++
Following all steps above, required files are ready in the tgemcell directory.
1. **dielectrics.dat** : First line tells the number of material(different volumes with same material are considered to have different material ID.) and the list of material IDs and its relative permitivity.
2. **mesh.boundary**, **mesh.elements**, **mesh.header** and **mesh.nodes** : geometry information.
3. **tgemcell.result** : field map solution.