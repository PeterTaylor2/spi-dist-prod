target:

U_SAMPLE_HOME=../..
include $(U_SAMPLE_HOME)/config.mk

vpath %.cpp ql
vpath %.cpp ql/utilities
vpath %.cpp ql/math

U_WIN32_DEBUG_CFLAGS=-D_SCL_SECURE_NO_WARNINGS
U_SRC_DIR=ql/time
U_INC_DIR=ql/time
U_INCLUDES=-I. -I$(G_QL_BOOST_HOME)
U_GENDEP_FILTERS=-f$(G_QL_BOOST_HOME)/boost
U_FORCE_REBUILD+=Makefile
U_VCPROJ_OPTIONS+=-S ql/utilities -S ql/math
U_VCPROJ_OPTIONS+=-H ql/utilities -H ql/math
U_OBJS=\
date.$(G_OBJ)\
frequency.$(G_OBJ)\
period.$(G_OBJ)\
timeunit.$(G_OBJ)\
weekday.$(G_OBJ)\
dataformatters.$(G_OBJ)\
errors.$(G_OBJ)\
matrix.$(G_OBJ)

U_TARGET=qlLite

include $(U_SPI_HOME)/makefiles/build/lib.mk

