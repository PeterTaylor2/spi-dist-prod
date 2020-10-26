target:

U_SAMPLE_HOME=../../..
include $(U_SAMPLE_HOME)/config.mk

U_ISDA_CDS=$(U_SAMPLE_HOME)/libs/isda-cds

U_INCLUDES=$(G_BOOST_INCLUDES)\
-I$(U_SAMPLE_HOME)/libs/cpplib -I..

U_DEP_LIBS=\
../cpplib-spi/$(G_BUILD_DIR)/$(G_LIB_PFX)cpplib-spi$(G_LIB_EXT)\
$(U_SAMPLE_HOME)/libs/cpplib/rates/$(G_BUILD_DIR)/$(G_LIB_PFX)cppRates$(G_LIB_EXT)\
$(U_SAMPLE_HOME)/libs/cpplib/maths/$(G_BUILD_DIR)/$(G_LIB_PFX)cppMaths$(G_LIB_EXT)\
$(U_SAMPLE_HOME)/libs/cpplib/utils/$(G_BUILD_DIR)/$(G_LIB_PFX)cppUtils$(G_LIB_EXT)

U_LIBS:=$(U_DEP_LIBS)

include $(U_SPI_HOME)/makefiles/spi/dll.mk


