target:

U_SAMPLE_HOME=../../..

include $(U_SAMPLE_HOME)/config.mk

U_SRC_DIR=src
U_INC_DIR=.
U_INCLUDES=$(G_BOOST_INCLUDES) -I$(U_SPI_HOME) -I../dll -I../../../libs/cpplib
U_FORCE_REBUILD=Makefile

U_TARGET=cpplib-spi

# this is necessary since this library is only ever going to be linked with
# cpplib-dll and any functions it uses from there will be exported
U_CFLAGS+=-DCPPLIB_EXPORT

include $(U_SPI_HOME)/makefiles/build/lib.mk

rstrip:
	@$(G_PYTHON) $(U_SPI_HOME)/makefiles/python/rstrip.py src/*.[ch]pp *.hpp

