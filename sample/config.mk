U_SPI_HOME=$(U_SAMPLE_HOME)/3rdParty/spi
NO_SVO2TEX=1
SARTORIAL_CONFIG?=win64
-include $(U_SAMPLE_HOME)/makefiles/config-$(COMPUTERNAME).mk
include $(U_SPI_HOME)/makefiles/config/config.mk

# for the simpler sample examples a small sub-set of boost is provided
# for more complicated samples a larger collection of boost is required
G_BOOST_INCLUDES=-I$(U_SAMPLE_HOME)
G_SPI_UTIL_INCLUDES=-I$(U_SPI_HOME)

# the directory below was created by use of the boost bcp tool
# it is too big to include in the sample package
# using boost directly out of the box caused problems with the BOOST_WORKAROUND macro
# so much so that we had to workaround it
#
# alternatively you could set G_QL_BOOST_HOME as follows:
# 1. download boost_1_70_0
# 2. copy files from sample/libs/qlLite/boost-overrides/boost into the boost directory
#    (i.e. boost_1_70_0/boost)
# 3. Set G_QL_BOOST_HOME to the location of boost_1_70_0 on your file system
G_QL_BOOST_HOME=$(U_SAMPLE_HOME)/../../boost-build/ql-boost

