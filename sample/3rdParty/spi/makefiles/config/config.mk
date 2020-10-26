#
# there is one environment variable that defines the build process
#
# this is SARTORIAL_CONFIG
#
# the makefile in the configs directory called $(SARTORIAL_CONFIG)_config.mk
# is opened - if it does nothing else then it must define G_BASE_CONFIG which
# defines the operating system and compiler in use
#
# modifications to the build process are defined by the command line
# variable DEBUG
#

DEBUG=0

ifeq ($(DEBUG),0)
I_MODE=release
else
ifeq ($(DEBUG),1)
I_MODE=debug
else
I_MODE=debug$(DEBUG)
endif
endif

###########################################################################
# The easiest way to define SARTORIAL_CONFIG and related variables is via
# the config/site.mk file. This is not part of the SPI package, but you
# can create your own local version and arrange that it does not get
# overwritten for new installations of SPI.
###########################################################################
-include $(U_SPI_HOME)/makefiles/config/site.mk
-include $(U_SPI_HOME)/makefiles/config/computers/$(COMPUTERNAME).mk
include $(U_SPI_HOME)/makefiles/config/configs/$(SARTORIAL_CONFIG)_config.mk

G_CONFIG:=$(G_BASE_CONFIG)_$(I_MODE)

include $(U_SPI_HOME)/makefiles/config/$(G_CONFIG).mk
include $(U_SPI_HOME)/makefiles/config/python.mk

info::
	@echo "COMPUTERNAME=$(COMPUTERNAME)"
	@echo "G_PLATFORM=$(G_PLATFORM)"
	@echo "G_BUILD_DIR=$(G_BUILD_DIR)"
	@echo "G_ABI=$(G_ABI)"
	@echo "SARTORIAL_CONFIG=$(SARTORIAL_CONFIG)"
	@echo "COMPILER=$(COMPILER)"
	@echo "WIN32_COMPILER=$(WIN32_COMPILER)"
	@echo "G_XL_VERSIONS=$(G_XL_VERSIONS)"
	@echo "G_PY_VERSIONS=$(G_PY_VERSIONS)"
	@echo "G_BITS=$(G_BITS)"
	@echo "G_BASE_CONFIG=$(G_BASE_CONFIG)"
	@echo "I_MODE=$(I_MODE)"
	@echo "G_PY_VERSION=$(G_PY_VERSION)"
	@echo "G_CONFIG=$(G_CONFIG)"
	@echo "G_PYTHON=$(G_PYTHON)"

