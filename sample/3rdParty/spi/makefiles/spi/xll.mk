############################################################################
# the main target is 'xll' instead of the more usual 'target'
#
# this is because we invoke the makefile more than once for each version
# of Excel we support, but ultimately it uses the dll.mk mechanism which
# expects something called 'target'
#
# the first block of commands are the ones which loop through the versions
#
# the second block of commands are the ones which run for a specific version
############################################################################

.PHONY: xll code target targets

############################################################################
# BLOCK ONE - commands which loop through the versions
############################################################################
xll:
	$(MAKE) targets

targets:
	@for xlVersion in $(G_XL_VERSIONS); do \
		echo "Building for XL_VERSION=$$xlVersion"; \
		$(MAKE) target XL_VERSION=$$xlVersion; \
	done

clean::
	@rm -fr $(G_BUILD_DIR)
	@for xlVersion in $(G_XL_VERSIONS); do \
		echo "Cleaning XL_VERSION=$$xlVersion"; \
		rm -fr $(G_BUILD_DIR)-xl$$xlVersion; \
		rm -fr ../$(G_ABI)/xl$$xlVersion; \
	done

vs-clean::
	@for xlVersion in $(G_XL_VERSIONS); do \
		echo "Cleaning XL_VERSION=$$xlVersion"; \
		rm -f $(G_BUILD_DIR)-xl$$xlVersion/*.$(G_OBJ); \
		rm -f $(G_BUILD_DIR)-xl$$xlVersion/*.d; \
		rm -fr ../$(G_ABI)/xl$$xlVersion; \
	done

############################################################################
# we need U_VCPROJ_OPTIONS for vcproj and vcxproj targets
############################################################################
U_VCPROJ_OPTIONS=-t xll
U_CONFIG_DIR?=../config
U_DLL_DIR?=../dll

-include $(U_DLL_DIR)/properties.mk

###########################################################################
# optional pre-build command for generating the code - usually done within
# the config directory
###########################################################################
code:
	$(MAKE) -C $(U_CONFIG_DIR) code-excel

U_FORCE_REBUILD+=Makefile $(U_DLL_DIR)/properties.mk
U_FORCE_REBUILD+=$(U_SPI_HOME)/makefiles/version.mk
###########################################################################
# the following values depend on properties.mk but can be overridden
###########################################################################
U_TARGET?=$(U_SERVICE_NAMESPACE)
U_VCPROJ?=$(U_SERVICE)-excel
U_CFLAGS+=-DXL_$(U_DECLSPEC)_EXPORT

############################################################################
# BLOCK TWO - commands to compile the code or install the extension for
# a specific excel version
#
# the first thing we do is try to weed out (based on version) which
# builds we don't actually run
############################################################################
ifneq ($(G_PLATFORM),win32)
U_NO_BUILD=1
else ifeq ($(G_BITS),64)
U_NO_BUILD=1
endif

############################################################################
# defining the build for given G_XL_VERSION
############################################################################
U_BUILD_SUFFIX=-xl$(G_XL_VERSION)
U_CFLAGS+=-DSPI_XL_VERSION=$(G_XL_VERSION)
U_SPI_BUILD_DIR?=$(G_BUILD_DIR)

include $(U_SPI_HOME)/makefiles/version.mk

U_SRC_DIR:=src
U_INC_DIR:=src
U_INCLUDES+=-I$(U_SPI_HOME) -I$(U_DLL_DIR)
ifndef U_XLADDIN_TARGET
U_XLADDIN_TARGET:=$(U_TARGET).xlam
endif

include $(U_SPI_HOME)/makefiles/spi/base_service.mk
include $(U_SPI_HOME)/makefiles/spi/runtime.mk

###########################################################################
# define the libraries including the SPI runtime libraries
#
# since this only works for windows we can use specific extensions to make
# things a bit easier to read
#
# we need the main DLL, spxl.lib and spi_util from the SPI runtime
# we need the service DLL
#
# probably no need for any extra DEP_LIBS to be user defined
###########################################################################
U_DEP_LIBS:=\
$(U_DLL_DIR)/$(G_BUILD_DIR)/$(U_SERVICE_DLL).lib\
$(U_DEP_LIBS)\
$(I_SPI_RUNTIME_BIN_DIR)/xl$(G_XL_VERSION)/$(SPI_DLL)-xl$(G_XL_VERSION).lib\
$(I_SPI_RUNTIME_BIN_DIR)/$(SPI_DLL).lib\
$(I_SPI_RUNTIME_BIN_DIR)/$(SPI_UTIL_DLL).lib

U_LIBS=$(U_DEP_LIBS) $(U_SPI_HOME)/excel/win32/xl$(G_XL_VERSION)/xlcall32.lib

###########################################################################
# regular DLL build except that we change the extension to be .xll instead
# of the standard .dll
###########################################################################
G_DLL_EXT:=.xll

ifneq ($(U_XLL_TARGET),)
U_TARGET:=$(U_XLL_TARGET)
else 
U_TARGET:=xl_$(U_TARGET)
endif

include $(U_SPI_HOME)/makefiles/build/dll.mk

###########################################################################
# what we do after the build
###########################################################################
post_build: dll xladdin

G_XL_ABI=$(G_ABI)/xl$(G_XL_VERSION)

dll: ../$(G_XL_ABI)/$(SPI_DLL).dll
dll: ../$(G_XL_ABI)/$(SPI_UTIL_DLL).dll
dll: ../$(G_XL_ABI)/$(U_SERVICE_DLL).dll
dll: ../$(G_XL_ABI)/$(U_TARGET).xll
dll: ../$(G_XL_ABI)/$(SPI_DLL)-xl$(G_XL_VERSION).dll

xladdin: ../$(G_XL_ABI)/$(U_XLADDIN_TARGET)

../$(G_XL_ABI)/$(SPI_DLL).dll: $(I_SPI_RUNTIME_BIN_DIR)/$(SPI_DLL).dll
	@mkdir -p ../$(G_XL_ABI)
	cp -pf $< ../$(G_XL_ABI)

../$(G_XL_ABI)/$(SPI_UTIL_DLL).dll: $(I_SPI_RUNTIME_BIN_DIR)/$(SPI_UTIL_DLL).dll
	@mkdir -p ../$(G_XL_ABI)
	cp -pf $< ../$(G_XL_ABI)

../$(G_XL_ABI)/$(SPI_DLL)-xl$(G_XL_VERSION).dll: $(I_SPI_RUNTIME_BIN_DIR)/xl$(G_XL_VERSION)/$(SPI_DLL)-xl$(G_XL_VERSION).dll
	@mkdir -p ../$(G_XL_ABI)
	cp -pf $< ../$(G_XL_ABI)

# why can't we use the copy of the U_SERVICE_DLL in G_ABI?
../$(G_XL_ABI)/$(U_SERVICE_DLL).dll: $(U_DLL_DIR)/$(G_BUILD_DIR)/$(U_SERVICE_DLL).dll
	@mkdir -p ../$(G_XL_ABI)
	cp -pf $< ../$(G_XL_ABI)

../$(G_XL_ABI)/$(U_TARGET)$(G_DLL_EXT): $(G_BUILD_DIR)-xl$(G_XL_VERSION)/$(U_TARGET)$(G_DLL_EXT)
	@mkdir -p ../$(G_XL_ABI)
	cp -pf $< ../$(G_XL_ABI)

../$(G_XL_ABI)/$(U_XLADDIN_TARGET): $(U_XLADDIN_TARGET)
	@mkdir -p ../$(G_XL_ABI)
	cp -pf $(U_XLADDIN_TARGET) ../$(G_XL_ABI)

# this rule => only have to define the extra DLLs to be copied in Makefile
../$(G_XL_ABI)/%.dll: ../$(G_ABI)/%.dll
	@mkdir -p ../$(G_XL_ABI)
	cp -pf $< ../$(G_XL_ABI)

# this rule => only have to define the extra PDFs to be copied in Makefile
../$(G_XL_ABI)/%.pdf: ../$(G_ABI)/%.pdf
	@mkdir -p ../$(G_XL_ABI)
	cp -pf $< ../$(G_XL_ABI)

