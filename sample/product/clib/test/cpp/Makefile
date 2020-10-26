# tests for the CLIB DLL
#
# use NAME= to define the test that you want to build and run
# use "make all" to build and run all tests

target:

U_SAMPLE_HOME=../../../..
include $(U_SAMPLE_HOME)/config.mk

NAME?=hello
OPTS=
U_TARGET=$(NAME)
U_VCPROJ=clib-test-cpp
U_VCPROJ_OPTIONS=-t all -j1

include $(U_SPI_HOME)/makefiles/version.mk
include $(U_SPI_HOME)/makefiles/spi/runtime.mk

U_OBJS = $(NAME).$(G_OBJ)

ifeq ($(G_PLATFORM),win32)

U_DEP_LIBS = \
../../dll/$(G_BUILD_DIR)/clib.lib\
$(I_SPI_RUNTIME_BIN_DIR)/$(SPI_DLL).lib\
$(I_SPI_RUNTIME_BIN_DIR)/$(SPI_UTIL_DLL).lib\
$(I_SPI_RUNTIME_BIN_DIR)/spi_boost.lib

PATH:=../../$(G_ABI):$(PATH)
export $PATH

else

U_DEP_LIBS = \
../../$(G_ABI)/libclib.so\
../../$(G_ABI)/lib$(SPI_DLL).so\
../../$(G_ABI)/lib$(SPI_UTIL_DLL).so\
$(I_SPI_RUNTIME_BIN_DIR)/$(G_LIB_PFX)spi_boost$(G_LIB_EXT)

LD_LIBRARY_PATH:=../../$(G_ABI):$(LD_LIBRARY_PATH)
export LD_LIBRARY_PATH

endif

U_LIBS = $(U_DEP_LIBS)
U_SRC_DIR = .
U_INC_DIR = .
U_INCLUDES = -I$(U_SPI_HOME) -I../../dll -I$(U_SAMPLE_HOME)

include $(U_SPI_HOME)/makefiles/build/exe.mk

debug:
	$(MAKE) run DEBUG=1 ARGS=-w

all:
	$(MAKE) clean
	sleep 1
	@for f in test*.cpp; do \
		name=`basename $$f .cpp`; \
		$(MAKE) -s run NAME=$$name; \
	done

rstrip:
	@$(G_PYTHON) $(U_SPI_HOME)/makefiles/python/rstrip.py *.cpp
