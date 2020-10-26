target:

U_SAMPLE_HOME=../../..
include $(U_SAMPLE_HOME)/config.mk

U_SRC_DIR=.
U_INC_DIR=.
U_INCLUDES=-I../include
U_GENDEP_FILTERS=-f../../../boost
U_FORCE_REBUILD=Makefile

U_TARGET=isdaErrorHandler

include $(U_SPI_HOME)/makefiles/build/lib.mk

rstrip:
	@$(G_PYTHON) $(U_SPI_HOME)/makefiles/python/rstrip.py *.[ch]pp

