include $(U_SPI_HOME)/makefiles/version.mk

U_INCLUDES+=-I$(U_SPI_HOME) -I../dll
U_SPI_BUILD_DIR?=$(G_BUILD_DIR)
U_DLL_DIR?=../dll
U_SRC_DIR?=.
U_INC_DIR?=.

include $(U_SPI_HOME)/makefiles/spi/base_service.mk
include $(U_SPI_HOME)/makefiles/spi/runtime.mk

-include $(U_DLL_DIR)/properties.mk

U_FORCE_REBUILD+=Makefile $(U_DLL_DIR)/properties.mk
U_FORCE_REBUILD+=$(U_SPI_HOME)/makefiles/version.mk
###########################################################################
# the following values depend on properties.mk but can be overridden
###########################################################################
U_TARGET?=$(U_SERVICE_NAMESPACE)-replay
U_VCPROJ?=$(U_SERVICE)-replay

ifeq ($(G_PLATFORM),win32)

U_DEP_LIBS:=\
$(U_DLL_DIR)/$(G_BUILD_DIR)/$(U_SERVICE_DLL).lib\
$(U_DEP_LIBS)\
$(I_SPI_RUNTIME_BIN_DIR)/$(G_LIB_PFX)$(SPI_DLL)$(G_LIB_EXT)\
$(I_SPI_RUNTIME_BIN_DIR)/$(G_LIB_PFX)$(SPI_UTIL_DLL)$(G_LIB_EXT)

U_LIBS=$(U_DEP_LIBS)

else

U_DEP_LIBS:=\
$(U_DLL_DIR)/$(G_BUILD_DIR)/lib$(U_SERVICE_DLL)$(G_DLL_EXT)\
$(U_DEP_LIBS)\
$(I_SPI_RUNTIME_BIN_DIR)/$(G_DLL_PFX)$(SPI_DLL)$(G_DLL_EXT)\
$(I_SPI_RUNTIME_BIN_DIR)/$(G_DLL_PFX)$(SPI_UTIL_DLL)$(G_DLL_EXT)

U_LIBS:=\
-L$(U_DLL_DIR)/$(G_BUILD_DIR) -l$(U_SERVICE_DLL)\
$(U_LIBS)\
-L$(I_SPI_RUNTIME_BIN_DIR) -l$(SPI_DLL) -l$(SPI_UTIL_DLL)

endif

include $(U_SPI_HOME)/makefiles/build/exe.mk

post_build: dll copy_exe

dll: ../$(G_ABI)/$(G_DLL_PFX)$(SPI_DLL)$(G_DLL_EXT)
dll: ../$(G_ABI)/$(G_DLL_PFX)$(SPI_UTIL_DLL)$(G_DLL_EXT)
dll: ../$(G_ABI)/$(G_DLL_PFX)$(U_SERVICE_DLL)$(G_DLL_EXT)

copy_exe: ../$(G_ABI)/$(U_TARGET)$(G_EXE)

../$(G_ABI)/$(G_DLL_PFX)$(SPI_DLL)$(G_DLL_EXT): $(I_SPI_RUNTIME_BIN_DIR)/$(G_DLL_PFX)$(SPI_DLL)$(G_DLL_EXT)
	@mkdir -p ../$(G_ABI)
	cp -pf $< ../$(G_ABI)
ifeq ($(G_PLATFORM),win32)
	@if [ -f $(basename $<).pdb ]; then cp -pf $(basename $<).pdb ../$(G_ABI); fi
endif

../$(G_ABI)/$(G_DLL_PFX)$(SPI_UTIL_DLL)$(G_DLL_EXT): $(I_SPI_RUNTIME_BIN_DIR)/$(G_DLL_PFX)$(SPI_UTIL_DLL)$(G_DLL_EXT)
	@mkdir -p ../$(G_ABI)
	cp -pf $< ../$(G_ABI)
ifeq ($(G_PLATFORM),win32)
	@if [ -f $(basename $<).pdb ]; then cp -pf $(basename $<).pdb ../$(G_ABI); fi
endif

../$(G_ABI)/$(G_DLL_PFX)$(U_SERVICE_DLL)$(G_DLL_EXT): $(U_DLL_DIR)/$(G_BUILD_DIR)/$(G_DLL_PFX)$(U_SERVICE_DLL)$(G_DLL_EXT)
	@mkdir -p ../$(G_ABI)
	cp -pf $< ../$(G_ABI)
ifeq ($(G_PLATFORM),win32)
	@if [ -f $(basename $<).pdb ]; then cp -pf $(basename $<).pdb ../$(G_ABI); fi
endif

../$(G_ABI)/$(U_TARGET)$(G_EXE): $(I_TARGET)
	@mkdir -p ../$(G_ABI)
	cp -pf $< ../$(G_ABI)
ifeq ($(G_PLATFORM),win32)
	@if [ -f $(basename $<).pdb ]; then cp -pf $(basename $<).pdb ../$(G_ABI); fi
endif

