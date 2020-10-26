############################################################################
# the main target is 'pyd' instead of the more usual 'target'
#
# this is because we invoke the makefile more than once for each version
# of python we support, but ultimately it uses the dll.mk mechanism which
# expects something called 'target'
#
# the first block of commands are the ones which loop through the versions
#
# the second block of commands are the ones which run for a specific version
############################################################################

.PHONY: pyd code target targets

include $(U_SPI_HOME)/makefiles/config/python.mk

############################################################################
# BLOCK ONE - commands which loop through the versions
############################################################################
pyd:
	$(MAKE) targets

targets:
	@for pyVersion in $(G_PY_VERSIONS); do \
		echo "Building for PY_VERSION=$$pyVersion"; \
		$(MAKE) target PY_VERSION=$$pyVersion; \
	done

install::
	@for pyVersion in $(G_PY_VERSION); do \
		echo "Installing for PY_VERSION=$$pyVersion"; \
		$(MAKE) install-py-version PY_VERSION=$$pyVersion; \
	done

clean::
	@rm -fr $(G_BUILD_DIR)
	@for pyVersion in $(G_PY_VERSIONS); do \
		echo "Cleaning PY_VERSION=$$pyVersion"; \
		rm -fr $(G_BUILD_DIR)-py$$pyVersion; \
		rm -fr ../$(G_ABI)/py$$pyVersion; \
	done

vs-clean::
	@for pyVersion in $(G_PY_VERSIONS); do \
		echo "Cleaning PY_VERSION=$$pyVersion"; \
		rm -f $(G_BUILD_DIR)-py$$pyVersion/*.$(G_OBJ); \
		rm -f $(G_BUILD_DIR)-py$$pyVersion/*.d; \
		rm -fr ../$(G_ABI)/py$$pyVersion; \
	done

############################################################################
# commands for creating the Visual Studio project files for installation
############################################################################
ifeq ($(G_PLATFORM),win32)

U_VCPROJ_OPTIONS+=-t pyd -s '*.py'

vcproj::
	@$(I_VCPROJ9_CMD) -t install $(U_VCPROJ)-install.vcproj $(U_VCPROJ)-install . . $(notdir $(I_TARGET))

v10.vcxproj::
	@$(I_VCPROJ10_CMD) -j1 -t install -b $(U_SPI_HOME)/build-win32/bin $(U_VCPROJ)-install.v10.vcxproj $(U_VCPROJ)-install . .

v14.vcxproj::
	@$(I_VCPROJ14_CMD) -j1 -t install -b $(U_SPI_HOME)/build-win32/bin $(U_VCPROJ)-install.v14.vcxproj $(U_VCPROJ)-install . .

v15.vcxproj::
	@$(I_VCPROJ15_CMD) -j1 -t install -b $(U_SPI_HOME)/build-win32/bin $(U_VCPROJ)-install.v15.vcxproj $(U_VCPROJ)-install . .

v16.vcxproj::
	@$(I_VCPROJ16_CMD) -j1 -t install -b $(U_SPI_HOME)/build-win32/bin $(U_VCPROJ)-install.v16.vcxproj $(U_VCPROJ)-install . .

endif

U_BUILD_SUFFIX=-py$(G_PY_VERSION)
U_SPI_BUILD_DIR?=$(G_BUILD_DIR)
I_SRC=src
U_DLL_DIR?=../dll
U_CONFIG_DIR?=../config

-include $(U_DLL_DIR)/properties.mk

###########################################################################
# optional pre-build command for generating the code - usually done within
# the config directory
###########################################################################
code:
	$(MAKE) -C $(U_CONFIG_DIR) code-python

U_FORCE_REBUILD+=Makefile $(U_DLL_DIR)/properties.mk
U_FORCE_REBUILD+=$(U_SPI_HOME)/makefiles/version.mk
############################################################################
# the following values depend on properties.mk but can be overridden
############################################################################
U_TARGET?=py_$(U_SERVICE_NAMESPACE)
U_VCPROJ?=$(U_SERVICE)-python
U_CFLAGS+=-DPY_$(U_DECLSPEC)_EXPORT
U_DEBUG_CFLAGS+=-DSPI_DEBUG

############################################################################
# BLOCK TWO - commands to compile the code or install the extension for
# a specific python version
#
# the first thing we do is try to weed out (based on version) which
# builds we don't actually run
############################################################################

ifeq ($(G_PLATFORM),win32)
ifeq ($(G_BITS),64)
ifneq ($(G_PY_VERSION),27)
ifneq ($(G_PY_VERSION),37)
U_NO_BUILD=1
endif
endif
endif
endif

include $(U_SPI_HOME)/makefiles/version.mk

U_SRC_DIR:=src
U_INC_DIR:=src
U_INCLUDES+=-I$(U_SPI_HOME) $(G_PYTHON_INCLUDES) -I$(U_DLL_DIR)

include $(U_SPI_HOME)/makefiles/spi/base_service.mk
include $(U_SPI_HOME)/makefiles/spi/runtime.mk

############################################################################
# define the libraries including the SPI runtime libraries
#
# we need both the main DLL and spi_util as well as sppy.lib from SPI
# we need the service DLL
#
# probably no need for extra DEP_LIBS to be user defined
############################################################################
ifeq ($(G_PLATFORM),win32)

U_DEP_LIBS:=\
$(U_DLL_DIR)/$(G_BUILD_DIR)/$(U_SERVICE_DLL).lib\
$(U_DEP_LIBS)\
$(I_SPI_RUNTIME_BIN_DIR)/py$(G_PY_VERSION)/$(SPI_DLL)-py$(G_PY_VERSION).lib\
$(I_SPI_RUNTIME_BIN_DIR)/$(SPI_DLL).lib\
$(I_SPI_RUNTIME_BIN_DIR)/$(SPI_UTIL_DLL).lib\
$(I_SPI_RUNTIME_BIN_DIR)/spi_boost.lib

U_LIBS=$(U_DEP_LIBS) $(G_PYTHON_LIBS)

G_DLL_EXT:=$(G_DEBUG_EXT).pyd

G_SPI_DLLS=$(SPI_DLL).dll $(SPI_UTIL_DLL).dll $(SPI_DLL)-py$(G_PY_VERSION).dll
G_INSTALL_DLL_EXT:=.dll
G_INSTALL_PYD_EXT:=.pyd
G_INSTALL_DLL_PFX:=
G_INSTALL_PYD_PFX:=

else

U_DEP_LIBS:=\
$(U_DLL_DIR)/$(G_BUILD_DIR)/lib$(U_SERVICE_DLL)$(G_DLL_EXT)\
$(U_DEP_LIBS)\
$(I_SPI_RUNTIME_BIN_DIR)/py$(G_PY_VERSION)/lib$(SPI_DLL)-py$(G_PY_VERSION)$(G_DLL_EXT)\
$(I_SPI_RUNTIME_BIN_DIR)/lib$(SPI_DLL)$(G_DLL_EXT)\
$(I_SPI_RUNTIME_BIN_DIR)/lib$(SPI_UTIL_DLL)$(G_DLL_EXT)\
$(I_SPI_RUNTIME_BIN_DIR)/libspi_boost$(G_LIB_EXT)

U_LIBS:=\
-L$(U_DLL_DIR)/$(G_BUILD_DIR) -l$(U_SERVICE_DLL)\
$(U_LIBS)\
-L$(I_SPI_RUNTIME_BIN_DIR)/py$(G_PY_VERSION) -l$(SPI_DLL)-py$(G_PY_VERSION)\
-L$(I_SPI_RUNTIME_BIN_DIR) -l$(SPI_DLL) -l$(SPI_UTIL_DLL)\
$(I_SPI_RUNTIME_BIN_DIR)/libspi_boost$(G_LIB_EXT)\
$(G_PYTHON_LIBS)

G_DLL_PFX=

G_SPI_DLLS:=lib$(SPI_DLL)$(G_DLL_EXT) lib$(SPI_UTIL_DLL)$(G_DLL_EXT) lib$(SPI_DLL)-py$(G_PY_VERSION)$(G_DLL_EXT)
G_INSTALL_DLL_EXT:=$(G_DLL_EXT)
G_INSTALL_PYD_EXT:=$(G_DLL_EXT)
G_INSTALL_DLL_PFX:=lib
G_INSTALL_PYD_PFX:=

ifeq ($(G_PLATFORM),macos64)

G_DLL_EXT:=.so
G_INSTALL_PYD_EXT:=.so

endif

endif

###########################################################################
# regular DLL build except that we change the extension for windows to be
# .pyd instead of the standard .dll
###########################################################################
include $(U_SPI_HOME)/makefiles/build/dll.mk

###########################################################################
# what we do after the build
###########################################################################
post_build: dll importer

G_PY_ABI=$(G_ABI)/py$(G_PY_VERSION)

importer: ../$(G_PY_ABI)/$(U_SERVICE_NAMESPACE).py

../$(G_PY_ABI)/$(U_SERVICE_NAMESPACE).py: src/$(U_SERVICE_NAMESPACE).py
	@mkdir -p ../$(G_PY_ABI)
	@rm -f ../$(G_PY_ABI)/*.pyc
	cp -pf $< ../$(G_PY_ABI)

# this rule => only have to define the extra PDFs to be copied in Makefile
../$(G_PY_ABI)/%.pdf: ../$(G_ABI)/%.pdf
	@mkdir -p ../$(G_PY_ABI)
	cp -pf $< ../$(G_PY_ABI)

ifeq ($(G_PLATFORM),win32)

dll: ../$(G_PY_ABI)/$(SPI_DLL).dll
dll: ../$(G_PY_ABI)/$(SPI_UTIL_DLL).dll
dll: ../$(G_PY_ABI)/$(U_SERVICE_DLL).dll
dll: ../$(G_PY_ABI)/$(U_TARGET)$(G_DLL_EXT)
dll: ../$(G_PY_ABI)/$(SPI_DLL)-py$(G_PY_VERSION).dll

../$(G_PY_ABI)/%.dll: $(I_SPI_RUNTIME_BIN_DIR)/%.dll
	@mkdir -p ../$(G_PY_ABI)
	cp -pf $< ../$(G_PY_ABI)

../$(G_PY_ABI)/%.dll: $(I_SPI_RUNTIME_BIN_DIR)/py$(G_PY_VERSION)/%.dll
	@mkdir -p ../$(G_PY_ABI)
	cp -pf $< ../$(G_PY_ABI)

# why can't we rely on G_ABI directory for this DLL?
../$(G_PY_ABI)/$(U_SERVICE_DLL).dll: $(U_DLL_DIR)/$(G_BUILD_DIR)/$(U_SERVICE_DLL).dll
	@mkdir -p ../$(G_PY_ABI)
	cp -pf $< ../$(G_PY_ABI)

../$(G_PY_ABI)/$(U_TARGET)$(G_DLL_EXT): $(G_BUILD_DIR)-py$(G_PY_VERSION)/$(U_TARGET)$(G_DLL_EXT)
	@mkdir -p ../$(G_PY_ABI)
	cp -pf $< ../$(G_PY_ABI)

# this rule => only have to define the extra DLLs to be copied in Makefile
../$(G_PY_ABI)/%.dll: ../$(G_ABI)/%.dll
	@mkdir -p ../$(G_PY_ABI)
	cp -pf $< ../$(G_PY_ABI)

else

dll: ../$(G_PY_ABI)/lib$(SPI_DLL)$(G_INSTALL_DLL_EXT)
dll: ../$(G_PY_ABI)/lib$(SPI_UTIL_DLL)$(G_INSTALL_DLL_EXT)
dll: ../$(G_PY_ABI)/lib$(U_SERVICE_DLL)$(G_INSTALL_DLL_EXT)
dll: ../$(G_PY_ABI)/$(U_TARGET)$(G_INSTALL_PYD_EXT)
dll: ../$(G_PY_ABI)/lib$(SPI_DLL)-py$(G_PY_VERSION)$(G_INSTALL_DLL_EXT)

../$(G_PY_ABI)/%$(G_INSTALL_DLL_EXT): $(I_SPI_RUNTIME_BIN_DIR)/%$(G_INSTALL_DLL_EXT)
	@mkdir -p ../$(G_PY_ABI)
	cp -pf $< ../$(G_PY_ABI)

../$(G_PY_ABI)/%$(G_INSTALL_DLL_EXT): $(I_SPI_RUNTIME_BIN_DIR)/py$(G_PY_VERSION)/%$(G_INSTALL_DLL_EXT)
	@mkdir -p ../$(G_PY_ABI)
	cp -pf $< ../$(G_PY_ABI)

# why can't we rely on G_ABI directory for this DLL?
../$(G_PY_ABI)/lib$(U_SERVICE_DLL)$(G_INSTALL_DLL_EXT): $(U_DLL_DIR)/$(G_BUILD_DIR)/lib$(U_SERVICE_DLL)$(G_INSTALL_DLL_EXT)
	@mkdir -p ../$(G_PY_ABI)
	cp -pf $< ../$(G_PY_ABI)

../$(G_PY_ABI)/$(U_TARGET)$(G_INSTALL_PYD_EXT): $(G_BUILD_DIR)-py$(G_PY_VERSION)/$(U_TARGET)$(G_INSTALL_PYD_EXT)
	@mkdir -p ../$(G_PY_ABI)
	cp -pf $< ../$(G_PY_ABI)

# this rule => only have to define the extra .so files to be copied in Makefile
../$(G_PY_ABI)/%$(G_INSTALL_DLL_EXT): ../$(G_ABI)/%$(G_INSTALL_DLL_EXT)
	@mkdir -p ../$(G_PY_ABI)
	cp -pf $< ../$(G_PY_ABI)

endif

###########################################################################
# commands for installing the build to site-packages
###########################################################################
ifdef U_NO_BUILD

install-py-version:
	@echo "No build for this platform - hence nothing to install"

else

ifeq ($(G_PLATFORM),win32)

install-py-version:
	$(G_PYTHON) $(U_SPI_HOME)/makefiles/python$(G_PY_MAJOR_VERSION)/installPython.py $(U_SERVICE_NAMESPACE) ../$(G_PY_ABI) $(U_INSTALL_FILES)

else

install-py-version::
	sudo $(G_PYTHON) $(U_SPI_HOME)/makefiles/python$(G_PY_MAJOR_VERSION)/installPython.py $(U_SERVICE_NAMESPACE) ../$(G_PY_ABI) $(U_INSTALL_FILES)

endif

endif 

###########################################################################
# commands for running python using the newly built library
###########################################################################
PYTHONPATH=../$(G_PY_ABI)
export PYTHONPATH

ifeq ($(G_PLATFORM),linux64)
LD_LIBRARY_PATH:=../$(G_PY_ABI):$(LD_LIBRARY_PATH)
export LD_LIBRARY_PATH
endif

run:
	$(WRAPPER) $(G_PYTHON) $(ARGS)

