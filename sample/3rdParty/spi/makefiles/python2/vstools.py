# various utility functions for dealing with visual studio issues

import os

def siteSettings():
    dn = os.path.join(
        os.path.dirname(os.path.dirname(os.path.abspath(__file__))),
        "config")
    computerName = os.getenv("COMPUTERNAME")
    fns = []
    if computerName:
        fns.append(os.path.join(dn, "computers", "%s.mk" % computerName))
    fns.append(os.path.join(dn, "site.mk"))
    fns.append(os.path.join(dn, "msvc_version.mk"))
    settings = {}
    for fn in fns:
        if not os.path.isfile(fn): continue
        with open(fn) as fp:
            for line in fp.readlines():
                if line.startswith("\t"): continue
                line = line.strip()
                if not line: continue
                if line.startswith("#"): continue
                line = line.split("#")[0]
                if not line: continue
                line = line.split("=")
                if len(line) == 2:
                    name = line[0]
                    value = line[1]
                    if name.endswith("?"):
                        name = name[:-1]
                        if name not in settings: settings[name] = value
                    else:
                        if name.endswith(":"): name = name[:-1]
                        settings[name] = value
    return settings

def systemIncludes(compiler, bits=32):

    if compiler == "msvc9":
        if bits == 32:
            return r"C:\Program Files (x86)\Microsoft Visual Studio 9.0\VC\include;C:\Program Files\Microsoft SDKs\Windows\v6.0A\Include"
        pass #end-if

    if compiler == "msvc10":
        if bits == 32:
            return r"C:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\include;C:\Program Files (x86)\Microsoft SDKs\Windows\v7.0A\Include"
        if bits == 64:
            # actually this depends on whether you are using commercial or express editions of VC10
            return r"C:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\include;C:\Program Files\Microsoft SDKs\Windows\v7.1\Include;C:\Program Files (x86)\Microsoft SDKs\Windows\v7.0A\Include"
        pass #end-if

    if compiler == "msvc14":
        settings = siteSettings()
        if bits in [32,64]:
            return r"C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\include;C:\Program Files (x86)\Windows Kits\10\include\%(G_VS14_KITS_VERSION)s\ucrt;C:\Program Files (x86)\Windows Kits\10\include\%(G_VS14_KITS_VERSION)s\um;C:\Program Files (x86)\Windows Kits\10\include\%(G_VS14_KITS_VERSION)s\shared" % settings
        pass #end-if

    if compiler == "msvc15":
        settings = siteSettings()
        if bits in [32,64]:
            return r"C:\Program Files (x86)\Microsoft Visual Studio\2017\%(G_VS15_PACKAGE_TYPE)s\VC\Tools\MSVC\%(G_VS15_TOOLS_VERSION)s\include;C:\Program Files (x86)\Windows Kits\10\include\%(G_VS15_KITS_VERSION)s\ucrt;C:\Program Files (x86)\Windows Kits\10\include\%(G_VS15_KITS_VERSION)s\um;C:\Program Files (x86)\Windows Kits\10\include\%(G_VS15_KITS_VERSION)s\shared" % settings

    if compiler == "msvc16":
        settings = siteSettings()
        if bits in [32,64]:
            return r"C:\Program Files (x86)\Microsoft Visual Studio\2019\%(G_VS16_PACKAGE_TYPE)s\VC\Tools\MSVC\%(G_VS16_TOOLS_VERSION)s\include;C:\Program Files (x86)\Windows Kits\10\include\%(G_VS16_KITS_VERSION)s\ucrt;C:\Program Files (x86)\Windows Kits\10\include\%(G_VS16_KITS_VERSION)s\um;C:\Program Files (x86)\Windows Kits\10\include\%(G_VS16_KITS_VERSION)s\shared" % settings

    raise Exception("Unsupported combination: COMPILER=%s BITS=%s" % (
            compiler, bits))

# executing make and capturing the result is more reliable but hideously slow
    #target = compiler[2:]
    #cmd = "make %s COMPILER=%s BITS=%s | grep INCLUDE=" % (
     #   target, compiler, bits)
    #result = os.popen(cmd).read().strip()
    #value = result.split("=",1)[1]
    #return value

def platforms(compiler):
    platforms = []
    # we don't have the system includes set-up for 64-bit msvc14 yet
    if compiler in ["msvc9"]:
        platforms.append(("Win32", 32))
    elif compiler in ["msvc10", "msvc14", "msvc15", "msvc16"]:
        platforms.append(("Win32", 32))
        platforms.append(("x64", 64))
        pass #end-if
    return platforms

if __name__ == "__main__":
    print (systemIncludes("msvc9"))
    print (systemIncludes("msvc14", bits=32))
    print (systemIncludes("msvc14", bits=64))
    print (systemIncludes("msvc15", bits=32))
    print (systemIncludes("msvc15", bits=64))
    print (systemIncludes("msvc16", bits=32))
    print (systemIncludes("msvc16", bits=64))
