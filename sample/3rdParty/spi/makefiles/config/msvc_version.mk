# version numbers for versions of Microsoft Visual Studio
# you can override these either via makefiles/config/site.mk or makefiles/config/computers/$(COMPUTERNAME).mk

G_VS14_MSSDK_VERSION?=v7.1A
G_VS14_KITS_VERSION?=10.0.10586.0
G_VS14_NETFXSDK_VERSION?=4.6.1

G_VS15_TOOLS_VERSION?=14.16.27023
G_VS15_KITS_VERSION?=10.0.17763.0
G_VS15_PACKAGE_TYPE?=Professional
G_VS15_NETFXSDK_VERSION?=4.6.1

G_VS16_TOOLS_VERSION?=14.24.28314
G_VS16_KITS_VERSION?=10.0.18362.0
G_VS16_PACKAGE_TYPE?=Professional
G_VS16_NETFXSDK_VERSION?=4.8

vc14::
	@echo MSSDK_VERSION=$(G_VS14_MSSDK_VERSION)
	@echo KITS_VERSION=$(G_VS14_KITS_VERSION)
	@echo NETFXSDK_VERSION=$(G_VS14_NETFXSDK_VERSION)
	@echo

vc15::
	@echo PACKAGE_TYPE=$(G_VS15_PACKAGE_TYPE)
	@echo TOOLS_VERSION=$(G_VS15_TOOLS_VERSION)
	@echo KITS_VERSION=$(G_VS15_KITS_VERSION)
	@echo NETFXSDK_VERSION=$(G_VS15_NETFXSDK_VERSION)
	@echo

vc16::
	@echo PACKAGE_TYPE=$(G_VS16_PACKAGE_TYPE)
	@echo TOOLS_VERSION=$(G_VS16_TOOLS_VERSION)
	@echo KITS_VERSION=$(G_VS16_KITS_VERSION)
	@echo NETFXSDK_VERSION=$(G_VS16_NETFXSDK_VERSION)
	@echo
