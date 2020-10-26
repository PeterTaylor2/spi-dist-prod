# makes a visual studio project file for VC10 (and above)

import uuid
import glob
import os

import vstools

def xmlfiles(filenames, label, filterName=None, indent=4):
    """
    Creates the XML block for representing a bunch of filenames
    """
    if len(filenames) == 0: return []
    indentString = " " * indent
    if filterName is None:
        lines = ['%s<%s Include="%s" />' % (indentString, label, filename)
                 for filename in filenames]
    else:
        lines = ['%s<%s Include="%s">\n%s  <Filter>%s</Filter>\n"%s</%s>' % (
            indentString, label, fileName,
            indentString, filterName,
            indentString, label)
                 for filename in filenames]
        pass #end-else
    lines.insert(0, "  <ItemGroup>")
    lines.append("  </ItemGroup>")
    return lines

def writeFilters(filename, filters, toolsVersion):
    ffn = filename + ".filters"
    nbFiles = 0
    for groupName,filterName,files in filters: nbFiles += len(files)
    if nbFiles == 0 and os.path.isfile(ffn):
        print ("removing", ffn)
        os.remove(ffn)
        return

    try:
        fp = open(ffn)
        oldContents = fp.read()
        fp.close()
    except: oldContents = ""

    oldLines = oldContents.split("\n")
    state = None
    uids = {}
    for line in oldLines:
        line = line.strip()
        if line.startswith("<Filter Include="):
            state = line.split('"')[1]
        elif line.startswith("</Filter>"):
            state = None
        elif line.startswith("<UniqueIdentifier>") and state is not None:
            uid = line.split("{")[1].split("}")[0]
            uids[state] = uid
            pass #end-if
        pass #end-for

    newLines = []
    newLines.append('<?xml version="1.0" encoding="utf-8"?>')
    newLines.append('<Project ToolsVersion="%s" xmlns="http://schemas.microsoft.com/developer/msbuild/2003">' % toolsVersion)

    started = False
    for groupName,filterName,files in filters:
        if len(files) == 0: continue
        if filterName is None: continue
        if filterName not in uids: uid = str(uuid.uuid4()).lower()
        else: uid = uids[filterName]
        if not started:
            newLines.append('  <ItemGroup>')
            started = True
            pass #end-if
        newLines.append('    <Filter Include="%s">' % filterName)
        newLines.append('      <UniqueIdentifier>{%s}</UniqueIdentifier>' % uid)
        newLines.append('    </Filter>')
        pass #end-if
    if started: newLines.append('  </ItemGroup>')

    for groupName,filterName,files in filters:
        if len(files) == 0: continue
        newLines.append('  <ItemGroup>')
        if filterName is None:
            for fn in files:
                newLines.append('    <%s Include="%s" />' % (groupName, fn))
                pass #end-for
            pass #end-if
        else:
            for fn in files:
                newLines.append('    <%s Include="%s">' % (groupName,fn))
                newLines.append('      <Filter>%s</Filter>' % filterName)
                newLines.append('    </%s>' % groupName)
                pass #end-for
            pass #end-else
        newLines.append('  </ItemGroup>')
        pass #end-for
    newLines.append('</Project>')
    newContents = "\n".join(newLines)

    if oldContents != newContents:
        print (ffn)
        fp = open(ffn, "w")
        fp.write(newContents)
        fp.close()
        pass #end-if
    pass #end-def

def getHeaderFiles(headerPatterns, incDir, srcDir, extraHeaderDirs):
    headerFiles = []
    for pattern in headerPatterns:
        headerFiles.extend(glob.glob(os.path.join(incDir,pattern)))
        if srcDir != incDir:
            headerFiles.extend(glob.glob(os.path.join(srcDir,pattern)))
        for dn in extraHeaderDirs:
            headerFiles.extend(glob.glob(os.path.join(dn, pattern)))
            pass #end-for
        pass #end-for
    return [os.path.normpath(fn) for fn in headerFiles]

def getSourceFiles(sourcePatterns, srcDir, extraSourceDirs):
    sourceFiles = []
    for pattern in sourcePatterns:
        sourceFiles.extend(glob.glob(os.path.join(srcDir, pattern)))
        for dn in extraSourceDirs:
            sourceFiles.extend(glob.glob(os.path.join(dn, pattern)))
            pass #end-for
        pass #end-for
    return [os.path.normpath(fn) for fn in sourceFiles]

def getBuildFiles():
    buildFiles = []
    if os.path.isfile("Makefile"): buildFiles.append("Makefile")
    buildFiles.extend(glob.glob("*.mk"))
    return buildFiles

def getItemGroupFiles(sourceFiles, headerFiles, buildFiles):
    itemGroupFiles = []
    itemGroupFiles.extend(xmlfiles(sourceFiles, "ClCompile"))
    itemGroupFiles.extend(xmlfiles(headerFiles, "ClInclude"))
    itemGroupFiles.extend(xmlfiles(buildFiles, "None"))
    return itemGroupFiles

def getProjectConfigurations(platforms):
    lines = []
    for platform in platforms:
        name = platform[0]
        bits = platform[1]
        for debug in ["Debug", "Release"]:
            lines.append("    <ProjectConfiguration Include=\"%s|%s\">" % (debug, name))
            lines.append("      <Configuration>%s</Configuration>" % debug)
            lines.append("      <Platform>%s</Platform>" % name)
            lines.append("    </ProjectConfiguration>")
    return "\n".join(lines)

def getPropertyConfigurations(platforms, platformToolset):
    lines = []
    for platform in platforms:
        name = platform[0]
        bits = platform[1]
        ## systemIncludes = vstools.systemIncludes(compiler, bits)
        for debug in ["Release", "Debug"]:
            lines.append("  <PropertyGroup Condition=\"'$(Configuration)|$(Platform)'=='%s|%s'\" Label=\"Configuration\">" % (debug, name))
            lines.append("    <ConfigurationType>Makefile</ConfigurationType>")
            if platformToolset is not None:
                lines.append("    <PlatformToolset>%s</PlatformToolset>" %
                             platformToolset)
                pass #end-if
            lines.append("  </PropertyGroup>")
    return "\n".join(lines)

def getImportPropertySheets(platforms):
    lines = []
    for platform in platforms:
        name = platform[0]
        for debug in ["Release", "Debug"]:
            lines.append("  <ImportGroup Condition=\"'$(Configuration)|$(Platform)'=='%s|%s'\" Label=\"PropertySheets\">" % (debug, name))
            lines.append("    <Import Project=\"$(UserRootDir)\Microsoft.Cpp.$(Platform).user.props\" Condition=\"exists('$(UserRootDir)\Microsoft.Cpp.$(Platform).user.props')\" Label=\"LocalAppDataPlatform\" />")
            lines.append("  </ImportGroup>")
            pass #end-for
        pass #end-for
    return "\n".join(lines)

def getPropertyGroups(platforms, makefileTarget, cleanTarget, compiler, bin,
                      buildSuffix, includePath, parallel, exeName, abiFunc):

    vsIncludePath = []
    for include in includePath:
        if include == ".": vsIncludePath.append("$(MSBuildProjectDirectory)")
        else: vsIncludePath.append(
            "$(MSBuildProjectDirectory)\\%s" % os.path.normpath(include))
    if len(vsIncludePath): vsIncludePath.append("")
    vsIncludePath = ";".join(vsIncludePath)

    lines = []
    parallelMake = "make" if parallel <= 1 else ("make -j%d" % parallel)
    for platform in platforms:
        name = platform[0]
        bits = platform[1]
        systemIncludes = vstools.systemIncludes(compiler, bits)
        for debug in ["Debug", "Release"]:
            debugFlag = " DEBUG=1" if debug == "Debug" else ""
            dn = "win%s\\%s" % (bits, debug)
            condition = "'$(Configuration)|$(Platform)'=='%s|%s'" % (debug,name)
            lines.append("    <OutDir Condition=\"%s\">%s\</OutDir>" % (
                    condition, dn))
            lines.append("    <IntDir Condition=\"%s\">%s\</IntDir>" % (
                    condition, dn))
            lines.append("    <NMakeBuildCommandLine Condition=\"%s\">%s %s COMPILER=%s BITS=%s%s</NMakeBuildCommandLine>" % (
                    condition, parallelMake, makefileTarget, compiler, bits, debugFlag))
            lines.append("    <NMakeReBuildCommandLine Condition=\"%s\">make %s COMPILER=%s BITS=%s%s &amp;&amp; %s %s COMPILER=%s BITS=%s%s</NMakeReBuildCommandLine>" % (
                    condition, cleanTarget, compiler, bits, debugFlag,
                    parallelMake, makefileTarget, compiler, bits, debugFlag))
            lines.append("    <NMakeCleanCommandLine Condition=\"%s\">make %s COMPILER=%s BITS=%s%s</NMakeCleanCommandLine>" % (
                    condition, cleanTarget, compiler, bits, debugFlag))
            lines.append("    <ExecutablePath Condition=\"%s\">%s;$(ExecutablePath)</ExecutablePath>" % (condition, os.path.normpath(bin)))
            if len(vsIncludePath):
                lines.append("    <NMakeIncludeSearchPath>%s%s</NMakeIncludeSearchPath>" % (
                        vsIncludePath, systemIncludes))

    if exeName:
        for platform in platforms:
            name = platform[0]
            bits = platform[1]
            for debug in ["Debug", "Release"]:
                abi = abiFunc(bits, debug) if abiFunc else "win%s\\%s" % (bits, debug)
                condition = "'$(Configuration)|$(Platform)'=='%s|%s'" % (debug,name)
                lines.append("  </PropertyGroup>")
                lines.append("  <PropertyGroup Condition=\"%s\">" % condition)
                lines.append("    <NMakeOutput>%s</NMakeOutput>" % (exeName.replace("G_ABI", abi)))

    return "\n".join(lines)

def makeProj(fileName, name, compiler, srcDir, incDir,
             includePath,
             headerPatterns, sourcePatterns,
             extraHeaderDirs, extraSourceDirs,
             toolsVersion,
             platformToolset,
             buildSuffix="",
             makefileTarget="target",
             cleanTarget="clean",
             bin=r"C:\cygwin\bin",
             exeName=None,
             abiFunc=None,
             parallel=4):

    headerFiles = getHeaderFiles(headerPatterns, incDir, srcDir, extraHeaderDirs)
    sourceFiles = getSourceFiles(sourcePatterns, srcDir, extraSourceDirs)

    buildFiles  = getBuildFiles()

    itemGroupFiles = []
    itemGroupFiles.extend(xmlfiles(sourceFiles, "ClCompile"))
    itemGroupFiles.extend(xmlfiles(headerFiles, "ClInclude"))
    itemGroupFiles.extend(xmlfiles(buildFiles, "None"))

    platforms = vstools.platforms(compiler)

    projectConfigurations  = getProjectConfigurations(platforms)
    propertyConfigurations = getPropertyConfigurations(
        platforms, platformToolset)
    importPropertySheets   = getImportPropertySheets(platforms)
    propertyGroups         = getPropertyGroups(
        platforms, makefileTarget, cleanTarget, compiler, bin,
        buildSuffix, includePath, parallel, exeName, abiFunc)

    data = {"name" : name,
            "tools_version" : toolsVersion}

    data["itemGroupFiles"] = "\n".join(itemGroupFiles)
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

    writeFilters(fileName,
                 [("ClCompile", "Source Files", sourceFiles),
                  ("ClInclude", "Header Files", headerFiles),
                  ("None", None, buildFiles)],
                 toolsVersion)
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
%(itemGroupFiles)s
  <Import Project="$(VCTargetsPath)\Microsoft.Cpp.targets" />
</Project>
"""

