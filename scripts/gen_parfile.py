import sys
import os

absPath = os.path.realpath(__file__)
absPath = absPath[:absPath.rfind("/")]
absPath = absPath[:absPath.rfind("/")]
sys.path.append(absPath + "/pysource/")
import parManager as pr

def isFloat(num):
    try:
        float(num)
        return True
    except ValueError:
        return False
def isInteger(num):
    try:
        int(num)
        return True
    except ValueError:
        return False

def PrintUsage():
    print(
"""
Usage : python3 gen_parfile.py <name> [parName=parValue]
    Possible optional parameter list (default value)
        E_DRIFT     : Drift field in V/cm (150)
        E_TRANS     : Transition field in V/cm (1500)
        E_INDUCTION : Induction field in V/cm (1500)
        B_Z         : Magnetic field in T (0)
        V_GEM1,2,3  : Voltage of GEM1,2,3 in V (100)
    Gas mixture name must be valid to load the gas file.
        GAS1,2      : Gas component names (He, iC4H10)
        FRAC1,2     : Fractions in percentage(90, 10)
        PRESSURE    : Gas pressure in torr(76)
    All distances are in the unit of mm
        DZ_DRIFT    : Height of constant E field(Drift field) (7.8)
        DZ_UG1      : Distance between the center of GEM1 and the boundary to constant E field (0.2)
        DZ_G12      : Distance between the center of GEM1 and GEM2 (0.2)
        DZ_G23      : Distance between the center of GEM2 and GEM3 (0.2)
        DZ_G3D      : Distance between the center of GEM3 and pad plane (0.2)
""")     

paramPack = {"SCRIPT_NAME" : "",
"E_DRIFT" : 150, "E_TRANS" : 1500, "E_INDUCTION" : 1500,
"B_Z" : 0.,
"V_GEM1" : 100, "V_GEM2" : 100, "V_GEM3" : 100,
"GAS1" : "He", "GAS2" : "iC4H10", "FRAC1" : 90, "FRAC2" : 10,
"PRESSURE" : 76,
"DZ_DRIFT" : 7.8, "DZ_UG1" : 0.2, "DZ_G12" : 0.2, "DZ_G23" : 0.2, "DZ_G3D" : 0.2}

if "-h" in sys.argv or "--help" in sys.argv or len(sys.argv) < 2:
    PrintUsage()
    exit()

# argument parsing
paramPack["SCRIPT_NAME"] = sys.argv[1]
for arg in sys.argv[2:]:
    if not "=" in arg:
        print("Invalid argument : " + arg)
        print("Use --help option to see the usage.")
        PrintUsage()
        exit()
    parName = arg[:arg.find("=")]
    parValue = arg[arg.find("=") + 1:]
    if not parName in paramPack:
        print("Invalid argument : " + arg)
        print("Use --help option to see the usage.")
        exit()
    else:
        if isInteger(parValue):
            paramPack[parName] = int(parValue)
        elif isFloat(parValue):
            paramPack[parName] = float(parValue)
        else:
            paramPack[parName] = parValue

# formatting SRIM and gas file
srimFileName = absPath + "/resources/SRIM/{GAS1}_{FRAC1:02d}_{GAS2}_{FRAC2:02d}_{PRESSURE:04d}.txt".format(**paramPack)
gasFileName = absPath + "/resources/gas_file/{GAS1}_{FRAC1:02d}_{GAS2}_{FRAC2:02d}_{PRESSURE:04d}.gas".format(**paramPack)

templateFile = open(absPath  + "/templates/parameter_template.txt", "r")
parLines = templateFile.readlines()
parTxt = "".join(parLines).format(**paramPack, SRIM_FILE=srimFileName, GAS_FILE=gasFileName)
templateFile.close()
parameterFile = open("./" + paramPack["SCRIPT_NAME"] + ".txt", "w")
parameterFile.write(parTxt)
parameterFile.close()