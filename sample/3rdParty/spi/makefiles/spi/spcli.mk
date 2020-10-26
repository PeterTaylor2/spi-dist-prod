############################################################################
# code generation makefile for .NET
#
# defines two targets: code-dotnet, clean-code-dotnet
#
# needs the following pre-defined macros
#
# U_CONFIG_DIR
# U_DOTNET_DIR
# U_SPI_HOME
# U_COMPANY_NAME
# U_COMPANY_NAMESPACE
#
# Optional extras:
#
# U_SPCLI_OPTIONS
#
# You should have included the generated file properties.mk before invoking
# these commands - these define the following:
#
# U_SERVICE
#
# You should have included the config directory for platform and compiler.
# These define the following:
#
# G_ABI
# G_EXE
#
############################################################################

.PHONY: code-dotnet clean-code-dotnet

I_DOTNET_SRC:=$(U_DOTNET_DIR)/src
I_DOTNET_TARGET:=$(U_DOTNET_DIR)/$(U_SERVICE).svo
I_DOTNET_SOURCE:=$(U_CONFIG_DIR)/$(U_SERVICE).svo

############################################################################
# code generation is driven by the timestamp for I_DOTNET_TARGET
############################################################################

###########################################################################
# If you have the copy of spcli corresponding to your current $(G_ABI)
# then use it - otherwise use the one in bin-$(G_PLATFORM) directory
###########################################################################
I_SPCLI:=$(wildcard $(U_SPI_HOME)/config/bin/$(G_ABI)/spcli$(G_EXE))
ifeq "$(I_SPCLI)" ""
I_SPCLI:=$(U_SPI_HOME)/config/bin-$(G_PLATFORM)/spcli$(G_EXE)
endif

code-dotnet: $(I_DOTNET_TARGET)

ifeq ($(G_PLATFORM),win32)

$(I_DOTNET_TARGET): $(I_DOTNET_SOURCE) $(I_SPCLI)
	@mkdir -p $(I_DOTNET_SRC)
	$(I_SPCLI) $(U_SPCLI_OPTIONS) $(I_DOTNET_SOURCE) $(I_DOTNET_TARGET) $(I_DOTNET_SRC) $(U_COMPANY_NAMESPACE) "$(U_COMPANY_NAME)"

else

$(I_DOTNET_TARGET):

endif

clean-code-dotnet:
	@rm -f $(I_DOTNET_TARGET)

