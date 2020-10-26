target:

U_SAMPLE_HOME=../../..
include $(U_SAMPLE_HOME)/config.mk

U_INCLUDES=-I$(G_QL_BOOST_HOME) -I$(U_SAMPLE_HOME)/libs/qlLite
U_WIN32_DEBUG_CFLAGS=-D_SCL_SECURE_NO_WARNINGS
U_SERVICE_DEBUG=QL_SPI_DEBUG

U_DEP_LIBS=\
$(U_SAMPLE_HOME)/libs/qlLite/$(G_BUILD_DIR)/$(G_LIB_PFX)qlLite$(G_LIB_EXT)

U_LIBS:=$(U_DEP_LIBS)

include $(U_SPI_HOME)/makefiles/spi/dll.mk

