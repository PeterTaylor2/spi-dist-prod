# makes a visual studio project file for VC9

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

def getPropertyGroups(platforms, target, makefileTarget, cleanTarget, bin,
                      compiler, includePath):

    vsIncludePath = []
    for include in includePath:
        if include == ".": vsIncludePath.append("$(MSBuildProjectDirectory)")
        else: vsIncludePath.append(
            "$(MSBuildProjectDirectory)\\%s" % os.path.normpath(include))
        pass #end-for
    if len(vsIncludePath): vsIncludePath.append("")
    vsIncludePath = ";".join(vsIncludePath)

    lines = []
    for platform in platforms:
        name = platform[0]
        bits = platform[1]
        systemIncludes = vstools.systemIncludes(compiler, bits)
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
            if len(vsIncludePath):
                lines.append("    <NMakeIncludeSearchPath>%s%s</NMakeIncludeSearchPath>" % (
                        vsIncludePath, systemIncludes))
    return "\n".join(lines)

def makeProj(fileName, name, target, srcDir,
             configPatterns, servicePatterns,
             includePath,
             compiler,
             toolsVersion, platformToolset, projectFileVersion,
             makefileTarget="code", bin=r"C:\cygwin\bin"):

    configFiles  = []
    serviceFiles = []
    for pattern in configPatterns:
        configFiles.extend(glob.glob(os.path.join(srcDir,pattern)))

    for pattern in servicePatterns:
        serviceFiles.extend(glob.glob(os.path.join(srcDir, pattern)))

    configFiles  = [os.path.normpath(fn) for fn in configFiles]
    serviceFiles = [os.path.normpath(fn) for fn in serviceFiles]
    serviceFiles.extend(makeVcxproj.getBuildFiles())

    platforms = vstools.platforms(compiler)

    projectConfigurations  = makeVcxproj.getProjectConfigurations(platforms)
    propertyConfigurations = makeVcxproj.getPropertyConfigurations(
        platforms, platformToolset)
    importPropertySheets   = makeVcxproj.getImportPropertySheets(platforms)
    propertyGroups         = getPropertyGroups(
        platforms, target, makefileTarget, "clean", bin, compiler, includePath)

    data = {"name" : name,
            "tools_version" : toolsVersion,
            "project_file_version" : projectFileVersion}

    data["configFiles"]  = xmlfiles(configFiles, "ClInclude")
    data["serviceFiles"] = xmlfiles(serviceFiles, "ClCompile")
    data["projectConfigurations"] = projectConfigurations
    data["propertyConfigurations"] = propertyConfigurations
    data["importPropertySheets"] = importPropertySheets
    data["propertyGroups"] = propertyGroups

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
    <_ProjectFileVersion>%(project_file_version)s</_ProjectFileVersion>
%(propertyGroups)s
  </PropertyGroup>
  <ItemDefinitionGroup>
  </ItemDefinitionGroup>
  <ItemGroup>
%(serviceFiles)s
  </ItemGroup>
  <ItemGroup>
%(configFiles)s
  </ItemGroup>
  <Import Project="$(VCTargetsPath)\Microsoft.Cpp.targets" />
</Project>
"""

