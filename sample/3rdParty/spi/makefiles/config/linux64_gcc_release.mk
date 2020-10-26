# definitions for building 64-bit libraries and applications on linux64
# using the gcc compiler

G_ABI=Release-linux64
G_BUILD_DIR=linux64/gcc_release

include $(U_SPI_HOME)/makefiles/config/linux64_gcc.mk

###########################################################################
# for C/C++ compilation we need to define the following:
#
# G_CFLAGS G_CPPFLAGS G_SYS_INCLUDES
# 
# since we set the system includes by defining INCLUDE we can skip setting
# G_SYS_INCLUDES
###########################################################################
G_OPTIMIZE_CFLAGS=-O3 -pipe
I_DEFINES = $(U_DEFINES) $(U_LINUX64_RELEASE_CFLAGS)
I_CFLAGS = -pthread -c -m64 -fPIC -Wformat -Wno-write-strings $(I_DEFINES)

G_CFLAGS = $(I_CFLAGS) 
G_CPPFLAGS = $(I_CFLAGS) -std=c++11

############################################################################
# Linker flags 
############################################################################
G_OPTIMIZE_LFLAGS = 
I_LFLAGS = -m64 -o$@ $(G_OPTIMIZE_LFLAGS) $(U_LINUX64_RELEASE_LFLAGS) -Wl,-rpath,.
I_DLL_LFLAGS = -shared $(G_LINKMAP_SPEC)

G_DLL_LFLAGS = $(I_LFLAGS) $(I_DLL_LFLAGS)
G_EXE_LFLAGS = $(I_LFLAGS) 

############################################################################
# Standard system libraries
############################################################################
G_SYS_LIBS = $(I_SYS_LIBS)

