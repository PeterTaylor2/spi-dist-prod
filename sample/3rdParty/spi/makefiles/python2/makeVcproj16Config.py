# makes a visual studio project file for VS16 for config files

import uuid
import glob
import os

import makeVcxprojConfig

def makeProj16(fileName, name, target, srcDir, configPatterns, servicePatterns,
               includePath, compiler,
               makefileTarget="all", bin=r"C:\cygwin\bin"):

    makeVcxprojConfig.makeProj(fileName, name, target, srcDir,
                               configPatterns,
                               servicePatterns, includePath, compiler,
                               "16.0", "v142", "UNKNOWN",
                               makefileTarget=makefileTarget, bin=bin)

if __name__ == "__main__":
    import sys
    import getopt

    opts,args = getopt.getopt(sys.argv[1:], "s:c:t:b:I:",
                              ["compiler="])
    configPatterns  = ["*.cfg"]
    servicePatterns = ["*.svc"]
    kwargs = {}
    includePath = []
    compiler = "msvc16"
    for opt in opts:
        if opt[0] == "-c": configPatterns.append(opt[1])
        elif opt[0] == "-s": servicePatterns.append(opt[1])
        elif opt[0] == "-t": kwargs["makefileTarget"] = opt[1]
        elif opt[0] == "-b": kwargs["bin"] = opt[1]
        elif opt[0] == "-I": includePath.append(opt[1])
        elif opt[0] == "--compiler": compiler = opt[1]
        pass #end-for

    if len(args) != 4:
        print (" ".join(sys.argv))
        raise Exception("Expecting 4 arguments: fileName name srcDir target")

    fileName = args[0]
    name     = args[1]
    srcDir   = args[2]
    target   = args[3]

    makeProj16(fileName, name, target, srcDir, configPatterns, servicePatterns,
               includePath, compiler,
               **kwargs)
    pass #end-def

