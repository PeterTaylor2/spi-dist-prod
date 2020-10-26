cli: target

.PHONY: cli code target

ifdef G_NO_DOTNET

U_NO_BUILD=1

clean::

else ifneq ($(G_PLATFORM),win32)

U_NO_BUILD=1

clean::

else

U_VCPROJ_OPTIONS=-t cli
U_SPI_BUILD_DIR?=$(G_BUILD_DIR)
U_CONFIG_DIR?=../config
U_DLL_DIR?=../dll

-include $(U_DLL_DIR)/properties.mk

###########################################################################
# optional pre-build command for generating the code - usually done within
# the config directory
###########################################################################
code:
	$(MAKE) -C $(U_CONFIG_DIR) code-dotnet

U_FORCE_REBUILD+=Makefile $(U_DLL_DIR)/properties.mk
U_FORCE_REBUILD+=$(U_SPI_HOME)/makefiles/version.mk
###########################################################################
# the following values depend on properties.mk but can be overridden
###########################################################################
U_TARGET?=$(U_SERVICE_NAMESPACE).net
U_VCPROJ?=$(U_SERVICE)-dotnet
U_VCPROJ_OPTIONS+=-s '*.inl'
U_CFLAGS+=-DCLI_$(U_DECLSPEC)_EXPORT

include $(U_SPI_HOME)/makefiles/version.mk

U_SRC_DIR:=src
U_INC_DIR:=src
U_INCLUDES+=-I$(U_SPI_HOME) -I$(U_DLL_DIR) 

include $(U_SPI_HOME)/makefiles/spi/base_service.mk
include $(U_SPI_HOME)/makefiles/spi/runtime.mk

###########################################################################
# define the libraries including the SPI runtime libraries
#
# we need the main DLL, spxl.lib and spi_util from the SPI runtime
# we need the service DLL
#
# probably no need for any extra DEP_LIBS to be user defined
###########################################################################
U_DEP_LIBS:=\
$(U_DLL_DIR)/$(G_BUILD_DIR)/$(G_LIB_PFX)$(U_SERVICE_DLL)$(G_LIB_EXT)\
$(U_DEP_LIBS)\
$(I_SPI_RUNTIME_BIN_DIR)/cliObject.$(G_OBJ)\
$(I_SPI_RUNTIME_BIN_DIR)/$(G_LIB_PFX)spcli$(G_LIB_EXT)\
$(I_SPI_RUNTIME_BIN_DIR)/$(G_LIB_PFX)$(SPI_DLL)$(G_LIB_EXT)\
$(I_SPI_RUNTIME_BIN_DIR)/$(G_LIB_PFX)$(SPI_UTIL_DLL)$(G_LIB_EXT)\
$(I_SPI_RUNTIME_BIN_DIR)/$(G_LIB_PFX)spi_boost$(G_LIB_EXT)\
$(I_SPI_RUNTIME_BIN_DIR)/$(G_LIB_PFX)spi_curl$(G_LIB_EXT)

U_LIBS:=$(U_DEP_LIBS) mscoree.lib msvcmrt.lib

###########################################################################
# regular commands for building a DLL
###########################################################################
include $(U_SPI_HOME)/makefiles/build/dll.mk

###########################################################################
# what we do after the build
#
# arguably there is no need to copy the service DLL and SPI DLL since
# these are handled by dll.mk
###########################################################################
post_build: xml dll

dll: ../$(G_ABI)/$(SPI_DLL).dll
dll: ../$(G_ABI)/$(SPI_UTIL_DLL).dll
dll: ../$(G_ABI)/$(U_SERVICE_DLL).dll 
dll: ../$(G_ABI)/$(U_TARGET)$(G_DLL_EXT)

xml: ../$(G_ABI)/$(U_TARGET).xml

../$(G_ABI)/$(U_TARGET).xml: src/$(U_TARGET).xml
	cp -pf src/$(U_TARGET).xml ../$(G_ABI)

../$(G_ABI)/$(SPI_DLL).dll: $(I_SPI_RUNTIME_BIN_DIR)/$(SPI_DLL).dll
	@mkdir -p ../$(G_ABI)
	cp -pf $< ../$(G_ABI)

../$(G_ABI)/$(SPI_UTIL_DLL).dll: $(I_SPI_RUNTIME_BIN_DIR)/$(SPI_UTIL_DLL).dll
	@mkdir -p ../$(G_ABI)
	cp -pf $< ../$(G_ABI)

../$(G_ABI)/$(U_SERVICE_DLL).dll: $(U_DLL_DIR)/$(G_BUILD_DIR)/$(U_SERVICE_DLL).dll
	@mkdir -p ../$(G_ABI)
	cp -pf $< ../$(G_ABI)

../$(G_ABI)/$(U_TARGET)$(G_DLL_EXT): $(G_BUILD_DIR)/$(U_TARGET)$(G_DLL_EXT)
	@mkdir -p ../$(G_ABI)
	cp -pf $< ../$(G_ABI)

###########################################################################
# extra clean commands (NB vs-clean needed for Visual Studio to avoid
# deleting directories in use by Visual Studio)
###########################################################################
clean::
	rm -f ../$(G_ABI)/*.net.*

vs-clean::
	rm -f ../$(G_ABI)/*.net.*

endif

