# win64_config.mk
#
# parameters for building on a 64-bit machine using by default the 64-bit
# VC16 compiler
#

BITS?=64

ifdef WIN32_COMPILER
COMPILER?=$(WIN32_COMPILER)
else
COMPILER?=msvc16
endif

G_WIN32_PROGRAM_FILES:=/cygdrive/c/Program Files (x86)
G_WIN64_PROGRAM_FILES:=/cygdrive/c/Program Files
G_BASE_CONFIG:=win$(BITS)_$(COMPILER)

