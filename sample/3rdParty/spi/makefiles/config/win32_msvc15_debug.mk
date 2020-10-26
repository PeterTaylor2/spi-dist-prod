# definitions for building 32-bit libraries and applications on Windows
# using the VC15 compiler

G_ABI=Debug-x86-vc15
G_BUILD_DIR=win32/msvc15_debug

include $(U_SPI_HOME)/makefiles/config/win32_msvc15.mk

###########################################################################
# for C/C++ compilation we need to define the following:
#
# G_CFLAGS G_CPPFLAGS G_SYS_INCLUDES
# 
# since we set the system includes by defining INCLUDE we can skip setting
# G_SYS_INCLUDES
###########################################################################

I_DEFINES = -D_CRT_SECURE_NO_WARNINGS $(U_DEFINES) $(U_DEBUG_CFLAGS) $(U_WIN32_DEBUG_CFLAGS)
I_CFLAGS = -c -MD -W3 -Gy -nologo -Od -FS -Fd$(I_BUILD_DIR)/$(U_TARGET).pdb $(I_DEFINES) 

# if we want multi-character byte set then add -D_MCBS

ifdef U_CLR

# we need to use -clr flag plus the 2.0 version of the .NET framework files
# also we must force use of C++ for C files

G_CPPFLAGS = $(I_CFLAGS) -Zi -EHa -clr $(G_CLR_USING)
G_CFLAGS = /TP $(G_CXX_CFLAGS)

else

G_CFLAGS = $(I_CFLAGS) -ZI
G_CPPFLAGS = $(I_CFLAGS) -ZI -EHsc

endif

############################################################################
# Linker flags - taken from a new visual studio project.
#
# /INCREMENTAL:NO Do not use incremental linking.
# /OPT:REF Removes unused data and functions.
# /OPT:ICF Number of iterations to use when searching for unused objects.
#          Default is equal to 2.
#
# Extra stuff from past experience:
#
# /NODEFAULTLIB To avoid problems with system library files.
# /FIXED:NO     This is standard behaviour for DLLs. Apparently for EXEs
#               FIXED:NO helps purify.
############################################################################
G_DEBUG_LFLAGS = /debug
I_LFLAGS = /profile /nodefaultlib /fixed:NO $(G_DEBUG_LFLAGS) $(U_WIN32_DEBUG_LFLAGS)

G_DLL_LFLAGS = $(I_LFLAGS) -dll
G_EXE_LFLAGS = $(I_LFLAGS) 

############################################################################
# Standard system libraries - since we use -MD instead of -MDd these are the
# same as the libraries for the optimised build
############################################################################
G_SYS_LIBS = msvcprt.lib msvcrt.lib vcruntime.lib ucrt.lib $(I_SYS_LIBS)
