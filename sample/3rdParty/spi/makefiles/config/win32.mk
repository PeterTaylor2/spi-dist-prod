#
# common build parameters for building 32-bit libraries and applications
# on Windows

###########################################################################
# Deal with the tedious complication of different file-naming styles.
###########################################################################
G_CYGWIN_ROOT?=/cygdrive/c

# G_WIN32_PROGRAM_FILES is a cygwin-style path (or should be)
# I_WIN32_PROGRAM_FILES defined below is the equivalent Windows style

I_WIN32_PROGRAM_FILES_DOS:=$(subst /,\,$(subst $(G_CYGWIN_ROOT),C:,$(G_WIN32_PROGRAM_FILES)))

include $(U_SPI_HOME)/makefiles/config/msvc_version.mk

G_PLATFORM=win32
G_BITS=32
G_EXTLIBS_HOME=$(U_SPI_HOME)/3rdParty

ifdef G_WIN32_XL_VERSIONS
G_XL_VERSIONS=$(G_WIN32_XL_VERSIONS)
else
G_XL_VERSIONS=4 12
endif

ifdef XL_VERSION
G_XL_VERSION=$(XL_VERSION)
else
G_XL_VERSION=4
endif

ifdef G_WIN32_PY_VERSIONS
G_PY_VERSIONS=$(G_WIN32_PY_VERSIONS)
else
G_PY_VERSIONS=27 37
endif

ifdef PY_VERSION
G_PY_VERSION=$(PY_VERSION)
else
ifdef G_WIN32_PY_VERSION
G_PY_VERSION=$(G_WIN32_PY_VERSION)
else
G_PY_VERSION=27
endif
endif

G_PYTHON_INCLUDES=-I$(U_SPI_HOME)/python/win32/include-py$(G_PY_VERSION)
G_PYTHON_LIBS=$(U_SPI_HOME)/python/win32/x86/python$(G_PY_VERSION)$(G_DEBUG_EXT).lib

SITE_PYTHON=G_WIN32_PYTHON$(G_PY_VERSION)
ifdef $(SITE_PYTHON)
G_PYTHON=$(realpath $($(SITE_PYTHON)))
# realpath has problems with spaces
ifeq ("$(G_PYTHON)","")
G_PYTHON=$($(SITE_PYTHON))
endif
else
G_PYTHON=C:/Python$(G_PY_VERSION)/python.exe
endif

site:
	@echo SITE_PYTHON=$(SITE_PYTHON) $($(SITE_PYTHON))
	@echo G_PYTHON=$(G_PYTHON)

ifdef DOTNET_VERSION
G_DOTNET_VERSION=$(DOTNET_VERSION)
else
G_DOTNET_VERSION=4.0.30319
endif

G_CLR_USING=\
/FU "c:\WINDOWS\Microsoft.NET\Framework\v$(G_DOTNET_VERSION)\System.dll" \
/FU "c:\WINDOWS\Microsoft.NET\Framework\v$(G_DOTNET_VERSION)\System.Data.dll" \
/FU "c:\WINDOWS\Microsoft.NET\Framework\v$(G_DOTNET_VERSION)\System.XML.dll"

