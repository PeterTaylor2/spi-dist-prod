# win32_config.mk
#
# parameters for building 32-bit libraries and applications on a 32-bit machine
# using VC10 as the default compiler
#
# the main issue is how to define the directory which contains the compiler
# which on a 32-bit machine will be installed in Program Files
#

ifdef WIN32_COMPILER
COMPILER?=$(WIN32_COMPILER)
else
COMPILER?=msvc10
endif

G_WIN32_PROGRAM_FILES:=/cygdrive/c/Program Files
G_BASE_CONFIG:=win32_$(COMPILER)

