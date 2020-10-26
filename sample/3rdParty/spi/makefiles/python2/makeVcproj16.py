# makes a visual studio project file for VC16

import uuid
import glob
import os

import makeVcxproj

# this needs to match the definitions in the makefiles/config directory
def abiFunc(bits, debug):
    if bits == 64:
        return "x64\\%s-vc16" % debug
    return "x86\\%s-vc16" % debug

def makeProj16(fileName, name, compiler, srcDir, incDir, includePath,
               headerPatterns, sourcePatterns,
               extraHeaderDirs, extraSourceDirs,
               buildSuffix="",
               makefileTarget="target",
               cleanTarget="clean",
               bin=r"C:\cygwin\bin",
               exe=None,
               parallel=4):

    makeVcxproj.makeProj(
        fileName, name, compiler, srcDir, incDir, includePath,
        headerPatterns, sourcePatterns, extraHeaderDirs, extraSourceDirs,
        "16.0", "v142",
        buildSuffix = buildSuffix,
        makefileTarget = makefileTarget,
        cleanTarget = cleanTarget,
        bin = bin,
        exeName = exe,
        abiFunc = abiFunc,
        parallel = parallel)

if __name__ == "__main__":
    import sys
    import getopt

    opts,args = getopt.getopt(sys.argv[1:], "h:s:c:t:b:H:S:I:j:",
                              ["buildSuffix=",
                               "exe=",
                               "makefileTarget=",
                               "cleanTarget="])
    headerPatterns  = ["*.hpp", "*.h"]
    sourcePatterns  = ["*.cpp", "*.c"]
    extraSourceDirs = []
    extraHeaderDirs = []
    compiler        = "msvc16"
    kwargs          = {}
    includePath     = []
    for opt in opts:
        if opt[0] == "-h": headerPatterns.append(opt[1])
        elif opt[0] == "-s": sourcePatterns.append(opt[1])
        elif opt[0] == "-H": extraHeaderDirs.append(opt[1])
        elif opt[0] == "-S": extraSourceDirs.append(opt[1])
        elif opt[0] == "-c": compiler = opt[1]
        elif opt[0] == "-t": kwargs["makefileTarget"] = opt[1]
        elif opt[0] == "--buildSuffix": kwargs["buildSuffix"] = opt[1]
        elif opt[0] == "--makefileTarget": kwargs["makefileTarget"] = opt[1]
        elif opt[0] == "--cleanTarget": kwargs["cleanTarget"] = opt[1]
        elif opt[0] == "--exe": kwargs["exe"] = opt[1]
        elif opt[0] == "-I": includePath.append(opt[1])
        elif opt[0] == "-b": kwargs["bin"] = opt[1]
        elif opt[0] == "-j": kwargs["parallel"] = int(opt[1])
        pass #end-for

    if len(args) != 4:
        print " ".join(sys.argv)
        raise Exception("Expecting 4 arguments: fileName name srcDir incDir")

    fileName = args[0]
    name     = args[1]
    srcDir   = args[2]
    incDir   = args[3]

    makeProj16(fileName, name, compiler, srcDir, incDir, includePath,
               headerPatterns, sourcePatterns,
               extraHeaderDirs, extraSourceDirs, **kwargs)
    pass #end-def

