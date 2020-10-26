#
# common build parameters for building 64-bit libraries and applications
# on Windows using the VC15 compiler
#
# our preference is to use /MD builds (regardless of whether we are in
# debug or release mode)
#
# these will link with the relevant run-time DLLs and hence all memory is
# actually allocated and managed within that particular run-time DLL
#
# this file will set all necessary path variables for using the compiler,
# so there is no need to rely on the path to be set externally
#
# note that the build system is assumed to be driven by cygwin - hence all
# file names are in cygwin style - when setting certain types of environment
# variables for use by Windows executables, then we need to ensure they will
# be in the correct Windows format
#
# cygwin itself translates PATH, but it doesn't know about variables such
# as INCLUDE or LIB - we want to use subst inside the makefile rather than
# calling cygpath since that is an unnecessary executable called for each
# invocation of make
#

###########################################################################
# Set the environment variables needed to get the compiler working. 
# 
# Deal with the tedious complication of different file-naming styles.
###########################################################################
include $(U_SPI_HOME)/makefiles/config/win64.mk

I_VS15_HOME:=$(G_WIN32_PROGRAM_FILES)/Microsoft Visual Studio/2017/$(G_VS15_PACKAGE_TYPE)
I_VS15_HOME_DOS:=$(I_WIN32_PROGRAM_FILES_DOS)\Microsoft Visual Studio\2017\$(G_VS15_PACKAGE_TYPE)

I_WINDOWS_KITS=$(I_WIN32_PROGRAM_FILES_DOS)\Windows Kits
I_WINDOWS_KITS_BASE_INCLUDE=$(I_WINDOWS_KITS)\10\include\$(G_VS15_KITS_VERSION)
I_WINDOWS_KITS_BASE_LIB=$(I_WINDOWS_KITS)\10\lib\$(G_VS15_KITS_VERSION)
I_WINDOWS_KITS_BIN=$(G_WIN32_PROGRAM_FILES)/Windows Kits/10/bin/$(G_VS15_KITS_VERSION)/x64

I_VC15_PATH:=$(I_VS15_HOME)/VC/Tools/MSVC/$(G_VS15_TOOLS_VERSION)/bin/Hostx86/x64:$(I_VS15_HOME)/Common7/IDE:$(I_WINDOWS_KITS_BIN)

PATH:=$(I_VC15_PATH):$(PATH)
INCLUDE:=$(I_VS15_HOME_DOS)\VC\Tools\MSVC\$(G_VS15_TOOLS_VERSION)\include;$(I_WINDOWS_KITS_BASE_INCLUDE)\ucrt;$(I_WINDOWS_KITS_BASE_INCLUDE)\um;$(I_WINDOWS_KITS_BASE_INCLUDE)\shared
LIB:=$(I_VS15_HOME_DOS)\VC\Tools\MSVC\$(G_VS15_TOOLS_VERSION)\lib\x64;$(I_WINDOWS_KITS_BASE_LIB)\ucrt\x64;$(I_WINDOWS_KITS_BASE_LIB)\um\x64;$(I_WINDOWS_KITS)\NETFXSDK\$(G_VS15_NETFXSDK_VERSION)\Lib\um\x64

export PATH
export INCLUDE
export LIB

vc15::
	@echo "PATH=$(PATH)"
	@echo
	@echo "INCLUDE=$(INCLUDE)"
	@echo
	@echo "LIB=$(LIB)"
	@echo
	@echo "where is the compiler:"
	@which cl
	@echo
	@echo "where is the library archiver:"
	@which lib
	@echo
	@echo "where is the linker:"
	@which link
	@echo
	@echo "where is the manifest builder:"
	@which mt


###########################################################################
# define standard variables to be used in all makefiles to avoid platform
# dependence
###########################################################################

G_OBJ=obj
G_EXE=.exe
G_LIB_PFX=
G_LIB_EXT=.lib
G_DLL_PFX=
G_DLL_EXT=.dll

G_CC=cl /Fo"$(I_BUILD_DIR)/" /nologo
G_CPPC=cl /Fo"$(I_BUILD_DIR)/" /nologo
G_LINK=link /out:"$@" /nologo
G_LIB=lib /out:"$@" /nologo

ifdef U_PRECOMPILED_HEADER
	G_PCH=$(I_BUILD_DIR)/$(U_PRECOMPILED_HEADER).pch
	G_PCH_OBJ=$(I_BUILD_DIR)/$(U_PRECOMPILED_HEADER).pch
	G_PCH_CC=cl.exe /Yc"$(U_PRECOMPILED_HEADER).hpp" /Fp"$(G_PCH) /Fo"$(G_PCH_OBJ)"
	G_PCH_CFLAGS=/Yu"$(U_PRECOMPILED_HEADER).hpp" /Fp"$(G_PCH)"
endif

# these are the system libraries independent of whether we are using
# debug or optimised code
I_SYS_LIBS=\
 kernel32.lib\
 wsock32.lib\
 netapi32.lib\
 advapi32.lib\
 oldnames.lib\
 comdlg32.lib\
 comctl32.lib\
 user32.lib\
 ws2_32.lib\
 wldap32.lib
