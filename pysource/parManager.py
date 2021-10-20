class ParManager:
    '''
    load a text file and initialize parameters
    :param fileName: file name including parameter information 
    :return:
    '''
    __instance = None
    __fileName = ""
    __paramMap = None
    __lineNum = 0
    def __init__(cls, fileName, print):
        if ParManager.__instance == None:
            ParManager.__instance = cls
            ParManager.__fileName = fileName
            ParManager.__printPars = print
    @classmethod
    def instance(cls, fileName, print = True):
        if ParManager.__instance == None:
            ParManager.__instance = cls(fileName, print)
        return ParManager.__instance

    @classmethod
    def initPars(cls):
        ParManager.__lineNum = 0
        ParManager.__paramMap = {}
        parName, parValue = "", ""
        file = open(ParManager.__fileName, "r")
        if ParManager.__printPars:
            print("File " + ParManager.__fileName +  " opened...")        
        # loop for parameter initializations
        while(True):
            if ParManager.__getNextPar(file) == False:
                break
        # print results
        if ParManager.__printPars:
            print("Registered %3d parameters"%len(ParManager.__paramMap))
            print("List of Parameters")
            print("Parameter name                 value")
            for key, value in ParManager.__paramMap.items():
                print("%-30s %s"%(key, value))
        file.close()
        if ParManager.__printPars:
            print("File " + ParManager.__fileName +  " closed...")

    @classmethod
    def insertParValuesInText(cls, txt):
        txt_out = txt.format(**ParManager.__paramMap)
        return txt_out
    
    @classmethod
    def getPar(cls, parName):
        try:
            return ParManager.__paramMap[parName]
        except KeyError:
            print("%s does not exist in the parameter list.")
            return 0
    
    @classmethod
    def listPars(cls):
        print("List of Parameters")
        print("Parameter name                 value")
        for key, value in ParManager.__paramMap.items():
            print("%-30s %s"%(key, value))        

    @classmethod
    def __getInstance(cls):
        return ParManager.__instance
    
    @classmethod
    def __getNextPar(cls, file):
        strLine = ""
        command = None
        while(True):
            strLine = file.readline()
            command = strLine[:strLine.find('#')].strip()
            valList = command.split()
            ParManager.__lineNum += 1
            # check end of the file            
            if not strLine:
                return False
            # check emtpy line
            elif not command:
                continue
            # check format
            elif len(valList) != 2:
                print("Cannot process line %5d of file %s : %s"%(ParManager.__lineNum, ParManager.__fileName, strLine))
                return False
            ParManager.__paramMap[valList[0]] = valList[1]
            return True


