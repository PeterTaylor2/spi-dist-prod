target:

U_SAMPLE_HOME=../../..
include $(U_SAMPLE_HOME)/config.mk

U_ISDA_CDS=$(U_SAMPLE_HOME)/libs/isda-cds

U_INCLUDES=$(G_BOOST_INCLUDES) -I$(U_ISDA_CDS)/include -I$(U_ISDA_CDS)/errorHandler -I$(U_SAMPLE_HOME)/libs

U_DEP_LIBS=\
$(U_ISDA_CDS)/errorHandler/$(G_BUILD_DIR)/$(G_LIB_PFX)isdaErrorHandler$(G_LIB_EXT)\
$(U_SAMPLE_HOME)/libs/isda-clib/$(G_BUILD_DIR)/$(G_LIB_PFX)isda-clib$(G_LIB_EXT)\
$(U_ISDA_CDS)/$(G_BUILD_DIR)/$(G_LIB_PFX)isda-cds$(G_LIB_EXT)

U_LIBS:=$(U_DEP_LIBS)

include $(U_SPI_HOME)/makefiles/spi/dll.mk


