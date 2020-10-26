# tests for the CLIB Python library
#
# use NAME= to define the test that you want to build and run
# use "make all" to build and run all tests

run:

U_PRODUCT_HOME=../..
U_SAMPLE_HOME=../../../..
U_TEST_NAME=cpplib-regression-test
LOGGING?=0

ifneq ($(LOGGING),0)
OPTS=--startup='import cpplib as clib; clib.start_logging("cpplib.log")'
endif

include $(U_SAMPLE_HOME)/config.mk
include $(U_SPI_HOME)/makefiles/regression-test/common.mk

run: run-one compare-one

all: run-all compare-all save-calls detail

find-calls:
	@$(FIND_CALLS) -b clib drivers

save-calls:
	@$(FIND_CALLS) -b -o cpplib-calls.csv clib drivers

