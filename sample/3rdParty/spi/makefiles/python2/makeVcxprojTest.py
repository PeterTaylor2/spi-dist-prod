# makes a visual studio project file for running regression tests

import uuid
import glob
import os

import makeVcxproj
import vstools

def xmlfiles(filenames, label, indent=4):
    """
    Creates the XML block for representing a bunch of filenames
    """
    indentString = " " * indent
    lines = ['%s<%s Include="%s"/>' % (indentString, label, filename)
             for filename in filenames]
    return "\n".join(lines)

def getPropertyGroups(platforms, makefileTarget, cleanTarget, bin, compiler):

    lines = []
    for platform in platforms:
        name = platform[0]
        bits = platform[1]
        for debug in ["Debug", "Release"]:
            debugFlag = " DEBUG=1" if debug == "Debug" else ""
            condition = "'$(Configuration)|$(Platform)'=='%s|%s'" % (debug,name)
            lines.append("    <NMakeBuildCommandLine Condition=\"%s\">make %s COMPILER=%s BITS=%s%s</NMakeBuildCommandLine>" % (
                condition, makefileTarget, compiler, bits, debugFlag))
            lines.append("    <NMakeReBuildCommandLine Condition=\"%s\">make %s COMPILER=%s BITS=%s%s &amp;&amp; make %s COMPILER=%s BITS=%s%s</NMakeReBuildCommandLine>" % (
                    condition, cleanTarget, compiler, bits, debugFlag,
                    makefileTarget, compiler, bits, debugFlag))
            lines.append("    <NMakeCleanCommandLine Condition=\"%s\">make %s COMPILER=%s BITS=%s%s</NMakeCleanCommandLine>" % (
                condition, cleanTarget, compiler, bits, debugFlag))
            lines.append("    <ExecutablePath Condition=\"%s\">%s;$(ExecutablePath)</ExecutablePath>" % (condition, os.path.normpath(bin)))
    return "\n".join(lines)

def makeProj(fileName, name,
             driverPatterns, inputPatterns, baselinePatterns,
             compiler,
             toolsVersion, platformToolset,
             makefileTarget="all", bin=r"C:\cygwin\bin"):

    driverFiles = []
    inputFiles = []
    baselineFiles = []
    for pattern in driverPatterns:
        driverFiles.extend(glob.glob(pattern))

    for pattern in inputPatterns:
        inputFiles.extend(glob.glob(pattern))

    for pattern in baselinePatterns:
        baselineFiles.extend(glob.glob(pattern))

    driverFiles   = [os.path.normpath(fn) for fn in driverFiles]
    inputFiles    = [os.path.normpath(fn) for fn in inputFiles]
    baselineFiles = [os.path.normpath(fn) for fn in baselineFiles]
    buildFiles    = makeVcxproj.getBuildFiles()

    platforms = vstools.platforms(compiler)

    projectConfigurations  = makeVcxproj.getProjectConfigurations(platforms)
    propertyConfigurations = makeVcxproj.getPropertyConfigurations(
        platforms, platformToolset)
    importPropertySheets   = makeVcxproj.getImportPropertySheets(platforms)
    propertyGroups         = getPropertyGroups(
        platforms, makefileTarget, "clean", bin, compiler)

    data = {"name" : name,
            "tools_version" : toolsVersion}

    allFiles = sorted(driverFiles + inputFiles + baselineFiles + buildFiles)

    data["allFiles"]               = xmlfiles(allFiles, "None")
    data["projectConfigurations"]  = projectConfigurations
    data["propertyConfigurations"] = propertyConfigurations
    data["importPropertySheets"]   = importPropertySheets
    data["propertyGroups"]         = propertyGroups

    try:
        fp = open(fileName)
        oldcontents = fp.read()
        fp.close()
    except: oldcontents = ""

    if "<ProjectGuid>" in oldcontents:
        guid = oldcontents.split("<ProjectGuid>")[1].split('<')[0]
        if guid.startswith("{") and guid.endswith("}"):
            guid = guid[1:-1]
        else: guid = None
    else: guid = None
    if guid is None: guid = str(uuid.uuid1()).upper()
    data["guid"] = guid

    global template
    contents = template % data

    if contents != oldcontents:
        print (fileName)
        fp = open(fileName, "w")
        fp.write(contents)
        fp.close()
        pass #end-if

    filters  = [("None", "baseline", sorted(baselineFiles)),
                ("None", "inputs", sorted(inputFiles)),
                ("None", "drivers", sorted(driverFiles)),
                ("None", None, sorted(buildFiles))]
    makeVcxproj.writeFilters(fileName, filters, toolsVersion)

    pass #end-def



template = """\
<?xml version="1.0" encoding="utf-8"?>
<Project DefaultTargets="Build" ToolsVersion="%(tools_version)s" xmlns="http://schemas.microsoft.com/developer/msbuild/2003">
  <ItemGroup Label="ProjectConfigurations">
%(projectConfigurations)s
  </ItemGroup>
  <PropertyGroup Label="Globals">
    <ProjectGuid>{%(guid)s}</ProjectGuid>
    <RootNamespace>%(name)s</RootNamespace>
    <ProjectName>%(name)s</ProjectName>
    <Keyword>MakeFileProj</Keyword>
  </PropertyGroup>
  <Import Project="$(VCTargetsPath)\Microsoft.Cpp.Default.props" />
%(propertyConfigurations)s
  <Import Project="$(VCTargetsPath)\Microsoft.Cpp.props" />
  <ImportGroup Label="ExtensionSettings">
  </ImportGroup>
%(importPropertySheets)s
  <PropertyGroup Label="UserMacros" />
  <PropertyGroup>
%(propertyGroups)s
  </PropertyGroup>
  <ItemDefinitionGroup>
  </ItemDefinitionGroup>
  <ItemGroup>
%(allFiles)s
  </ItemGroup>
  <Import Project="$(VCTargetsPath)\Microsoft.Cpp.targets" />
</Project>
"""

