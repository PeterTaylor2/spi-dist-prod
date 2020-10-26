# common build parameters for building 64-bit libraries and applications
# on linux64

G_PLATFORM=linux64
G_BITS=64
G_EXTLIBS_HOME=$(U_SPI_HOME)/3rdParty

G_PY_VERSIONS=2.7

ifdef PY_VERSION
G_PY_VERSION=$(PY_VERSION)
else
G_PY_VERSION=2.7
endif
G_PYTHON=/usr/bin/python
G_PYTHON_INCLUDES=-I/usr/include/python$(G_PY_VERSION)
G_PYTHON_LIBS=-L/usr/lib/x86_64-linux-gnu -lpython$(G_PY_VERSION)
G_CURL_LIBS=-L/usr/lib/x86_64-linux-gnu -lcurl

