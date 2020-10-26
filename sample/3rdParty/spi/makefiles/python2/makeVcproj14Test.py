# makes a visual studio project file for VS14 for config files

import uuid
import glob
import os

import makeVcxprojTest

def makeProj14(fileName, name, driverPatterns, inputPatterns, baselinePatterns,
               compiler,
               makefileTarget="all", bin=r"C:\cygwin\bin"):

    makeVcxprojTest.makeProj(fileName, name,
                             driverPatterns, inputPatterns, baselinePatterns,
                             compiler,
                             "14.0", "v140",
                             makefileTarget=makefileTarget, bin=bin)

if __name__ == "__main__":
    import sys
    import getopt

    opts,args = getopt.getopt(sys.argv[1:], "i:o:d:t:b:",
                              ["compiler="])
    driverPatterns   = ["drivers/*.py"]
    inputPatterns    = ["inputs/*.inp"]
    baselinePatterns = ["baseline/*.out"]
    kwargs = {}
    compiler = "msvc14"
    for opt in opts:
        if opt[0] == "-d": driverPatterns.append(opt[1])
        elif opt[0] == "-i": inputPatterns.append(opt[1])
        elif opt[0] == "-o": baselinePatterns.append(opt[1])
        elif opt[0] == "-t": kwargs["makefileTarget"] = opt[1]
        elif opt[0] == "-b": kwargs["bin"] = opt[1]
        elif opt[0] == "--compiler": compiler = opt[1]
        pass #end-for

    if len(args) != 2:
        print " ".join(sys.argv)
        raise Exception("Expecting 2 arguments: fileName name")

    fileName = args[0]
    name     = args[1]

    makeProj14(fileName, name, driverPatterns, inputPatterns, baselinePatterns,
               compiler, **kwargs)
    pass #end-def

