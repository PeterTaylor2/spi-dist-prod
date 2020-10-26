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

include $(U_SPI_HOME)/makefiles/version.mk

U_OBJS = $(NAME).$(G_OBJ)

ifeq ($(G_PLATFORM),win32)

U_DEP_LIBS = \
../../dll/$(G_BUILD_DIR)/clib.lib\
$(U_SPI_HOME)/dll/$(G_BUILD_DIR)/$(SPI_DLL).lib\
$(U_SPI_HOME)/spi_util/$(G_BUILD_DIR)/spi_util.lib 

PATH:=../../$(G_ABI):$(PATH)
export $PATH

else

U_DEP_LIBS = \
../../$(G_ABI)/libclib.so\
../../$(G_ABI)/lib$(SPI_DLL).so\
$(U_SPI_HOME)/spi_util/$(G_BUILD_DIR)/libspi_util.a

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
