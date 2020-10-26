Notes on configuration for the Windows build
============================================

1. You need an installation of Visual Studio (either 2017 or 2019) to run the build.
2. You need an installation of Python (either 2.7 or 3.7) to run the tests.
3. You need to write configuration files to say where these installations are found on your computer.

There is a file that will be optionally include when doing the builds, namely:

sample/makefiles/config-$(COMPUTERNAME).mk

If it doesn't exist then the file is ignored.
COMPUTERNAME is the environment variable defined by Windows which tells you them name of your PC.
This allows you to configure different computers in an organisation separately.

We will assume that Visual Studio has been installed in the normal locations.
However the version numbers will depend upon on how recently you have upgraded your compiler.
In addition the edition of the compiler (e.g. Professional or Community) is needed.

See the file spi/makefiles/config/msvc_version.mk for the default definitions of variables.
For Visual Studio 2017 it is the variables starting with G_VS15.
For Visual Studio 2019 it is the variables starting with G_VS17.

You can define those variables in either the site.mk file (if there is a common set-up for all PCs in your
organisation), or else in the computer-specific file.

Here is some example usage for a computer with only the Community edition and an earlier version which has not
been upgraded yet:

G_VS16_PACKAGE_TYPE:=Community
G_VS16_NETFXSDK_VERSION:=4.7.2
G_VS16_TOOLS_VERSION:=14.21.27702
G_VS16_KITS_VERSION:=10.0.17763.0

Note in this example we use := in the makefile to ensure the variable over-rides anything defined earlier.
You may have noted in the msvc_version.mk the notation used is ?= which means only use this value if it has
not been defined already.

For python versions used there are some variables in the makefile system as follows:

G_WIN32_PYTHON27
G_WIN32_PYTHON37
G_WIN64_PYTHON27
G_WIN64_PYTHON37

We also define

G_WIN32_PY_VERSION
G_WIN32_PY_VERSIONS
G_WIN64_PY_VERSION
G_WIN64_PY_VERSIONS

See the file example.txt in spi/makefiles/config/computers for an example of a possible computer specific configuration.
