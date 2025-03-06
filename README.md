Written by Hyunmin Yang & Haein Lee, HANUL, Korea University.
Updated by HeeJeong BYEON, HNP, Kyungpook National University

This program simulates TPC(Time Projection Chamber) with tripple GEMs.
Interation between charged particles and gas medium will be simulated with Geant4.
Drift and multiplication of electrons ionized by charged particle will be simulated with Garfield++.

# Environment
Ubuntu or MacOs.

# Prerequisites
- ROOT version 6  
- Garfield++  
- Geant4  
- Gmsh  
- Elmer

# parameters
All physical information(electric field, GEM voltages and geometry) is managed by parManager class(both c++ and python).
If you want to modify it, you must write a text file including all.</br>
To find out the format of configuration, refer to **parameters/params.txt**. Interfaces to a parameter text file are parManager.py and parManager.cpp for python3 and c++ each other.

# templates
All text files are templated in python compatiable format. Parameters are replaced their names enclosed by curly braces so that it is possible easily replace by wanted value by loading as python string and calling format() method. 

# scripts
All script files are located in **scripts** directory
- gen_parfile.py    :   Generates parameter files taking arguments ad inserting them into templates/parameter_template.txt. Type --help option to see its usage.
- gen_scripts.py    :   Generates input configuration files for Elmer solver(.geo, .sif and dielectrics.dat), reading information from a given parameter file.
- gen_fieldmap.py   :   Executes Gmsh and ElmerGrid, ElmerSolver to generate mesh and field map data file.
- gen_all.sh        :   Executes two scripts above in once and delete unnecessary files.

# Executables
- tgem
- tgem_gain
- view_tgem

# Garfield++

## Generating a parameter cofigure file
First of all, you must generate a parameter configure file.
```
mkdir build
cd build
cmake ..
python3 scripts/gen_parfiles.py <SCRIPT_NAME> [PARNAME1=PARVALUE1] ... [PARNAMEN=PARVALUEN]
```
All pairs of parameter name and value are inserted to templates/parameter_template.txt and the missing value will be set by the default values.
Type --help to see detailed information.

## Generating scripts
Next, you need to generate script files as input for Gmsh and ElmerGrid, Elmersolver.
By running script **gen_scripts.py**, parameters listed in a parameter file will be inserted into script templets **geometry.py**, **startinfo.py** and **dielectrics.py**.
```
python3 scripts/gen_scripts.py <SCRIPT_NAME> -all
```
First input is a parameter file containing full information of this project. **SCRIPT_NAME** must be defined in the given parameter file.
After running, **[SCRIPT_NAME].geo**, **[SCRIPT_NAME].sif** and **[SCRIPT_NAME/]dielectrics.dat** will be created on your directory.
## Meshing
Next, you have to generate a 3D mesh file describing unit cell of tripple GEM.
```
gmsh [SCRIPT_NAME].geo -3 -order 2 -format msh2 -o test.msh
```
After running, **[SCRIPT_NAME].msh** will be created on your directory.
## Generating field map data.
```
ElmerGrid 14 2 [SCRIPT_NAME].msh -autoclean
ElmerSolver [SCRIPT_NAME].sif
```
or running the single scripts.
```
python3 scripts/gen_fieldmap.py <SCRIPT_NAME>
```
ElmerGrid converts .msh file to **mesh.boundary**, **mesh.elements**, **mesh.header** and **mesh.nodes** in the **SCRIPT_NAME** diretory. The second command solves the field equation using **[SCRIPT_NAME].sif**(start information file).
**[SCRIPT_NAME].ep** and **[SCRIPT_NAME].result** will be created the **SCRIPT_NAME** directory.

You can go through the two steps above with one script.
```
source scripts/gen_all.sh <SCRIPT_NAME>
```
## Importing field map data to Garfield++
Following all steps above, required files are ready in the SCRIPT_NAME directory.
1. **SCRIPT_NAME.txt** : Generated paramter configure file.
2. **SCRIPT_NAME/dielectrics.dat** : First line tells the number of material(different volumes with same material are considered to have different material ID.) and the list of material IDs and its relative permitivity.
3. **SCRIPT_NAME/mesh.boundary**, **SCRIPT_NAME/mesh.elements**, **SCRIPT_NAME/mesh.header** and **mesh.nodes** : geometry information.
4. **SCRIPT_NAME/SCRIPT_NAME.result** : field map solution.
