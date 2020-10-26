doc:
	@$(MAKE) tex
	@$(MAKE) pdf

.PHONY: tex pdf doc

include $(U_SPI_HOME)/makefiles/version.mk

###########################################################################
# commands for creating the tex file from the .svo file
###########################################################################
U_SPTEX?=sptex

I_SPTEX=$(wildcard $(U_SPI_HOME)/config/bin-win32/$(G_ABI)/$(U_SPTEX)$(G_EXE))
ifeq "$(I_SPTEX)" ""
I_SPTEX:=$(U_SPI_HOME)/config/bin-win32/$(U_SPTEX)$(G_EXE)
endif

I_SVO2TEX_SCRIPTS=$(U_SPI_HOME)/svo/svo2tex$(G_PY_MAJOR_VERSION).py $(wildcard $(U_SPI_HOME)/svo/lib$(G_PY_MAJOR_VERSION)/*.py)
U_SPDOC_BIN=$(U_SPI_HOME)/svo/spdoc/bin/$(G_ABI)-py$(G_PY_VERSION)
PYTHONPATH=$(U_SPDOC_BIN)
export PYTHONPATH

ifeq ($(G_PLATFORM),linux64)
LD_LIBRARY_PATH:=$(U_SPDOC_BIN):$(LD_LIBRARY_PATH)
export LD_LIBRARY_PATH
endif

ifeq ($(G_PLATFORM),macos64)
DYLD_LIBRARY_PATH:=$(U_SPDOC_BIN):$(DYLD_LIBRARY_PATH)
export DYLD_LIBRARY_PATH
endif

U_VCPROJ?=$(U_SERVICE)-doc
U_CONFIG_DIR?=../config

tex: $(U_SERVICE).svo

ifdef NO_SVO2TEX

$(U_SERVICE).svo: $(U_CONFIG_DIR)/$(U_SERVICE).svo $(I_SPTEX)
	@mkdir -p tex
	@mkdir -p tex_imports
	$(I_SPTEX) $(U_SPTEX_OPTIONS) $(U_CONFIG_DIR)/$(U_SERVICE).svo $(U_SERVICE).svo .

else

$(U_SERVICE).svo: $(U_CONFIG_DIR)/$(U_SERVICE).svo $(I_SVO2TEX_SCRIPTS)
	@mkdir -p tex
	@mkdir -p tex_imports
	@echo PYTHONPATH=$(PYTHONPATH)
	$(G_PYTHON) $(U_SPI_HOME)/svo/svo2tex$(G_PY_MAJOR_VERSION).py $(U_SPTEX_OPTIONS) $(U_CONFIG_DIR)/$(U_SERVICE).svo $(U_SERVICE).svo .

endif

###########################################################################
# commands for creating the PDF file from the generated .tex files and the
# master .tex file
###########################################################################

ifdef G_PDFLATEX_BIN_DIR
PATH:=$(G_PDFLATEX_BIN_DIR):$(PATH)
export PATH
endif

U_SERVICE_DOC?=$(U_SERVICE)

pdf: ../$(G_ABI)/$(U_SERVICE_DOC).pdf

ifeq ($(G_PLATFORM),win32)

$(U_SERVICE_DOC).pdf: $(U_SERVICE).svo $(U_SERVICE_DOC).tex
	@rm -f *.aux *.bbl *.blg *.log *.out *.toc
	@pdflatex --quiet $(U_SERVICE_DOC).tex
	@pdflatex --quiet $(U_SERVICE_DOC).tex
	pdflatex --quiet $(U_SERVICE_DOC).tex
	@rm -f *.aux *.bbl *.blg *.log *.out *.toc

else

$(U_SERVICE_DOC).pdf: $(U_SERVICE).svo $(U_SERVICE_DOC).tex
	@rm -f *.aux *.bbl *.blg *.log *.out *.toc
	@echo "pdflatex $(U_SERVICE_DOC).tex"
	@pdflatex $(U_SERVICE_DOC).tex > /dev/null
	@pdflatex $(U_SERVICE_DOC).tex > /dev/null
	@pdflatex $(U_SERVICE_DOC).tex > /dev/null
	@rm -f *.aux *.bbl *.blg *.log *.out *.toc

endif

../$(G_ABI)/$(U_SERVICE_DOC).pdf: $(U_SERVICE_DOC).pdf
	@mkdir -p ../$(G_ABI)
	cp -pf $< ../$(G_ABI)

###########################################################################
# clean-up command
###########################################################################
clean:
	rm -f $(U_SERVICE).svo tex/*.tex $(U_SERVICE_DOC).pdf
	@rm -f *.aux *.bbl *.blg *.dvi *.log *.out *.toc

###########################################################################
# visual studio project files
###########################################################################
vcproj:
	@$(G_PYTHON) $(U_SPI_HOME)/makefiles/python$(G_PY_MAJOR_VERSION)/makeVcproj9Config.py -t doc -s ../doc/$(U_SERVICE_DOC).tex $(U_VCPROJ).vcproj $(U_VCPROJ) $(U_CONFIG_DIR) $(U_SERVICE_DOC).pdf

v10.vcxproj:
	@$(G_PYTHON) $(U_SPI_HOME)/makefiles/python$(G_PY_MAJOR_VERSION)/makeVcproj10Config.py $(U_VCXPROJ_OPTIONS) -b $(U_SPI_HOME)/build-win32/bin -t doc $(U_VCPROJ).v10.vcxproj $(U_VCPROJ) $(U_CONFIG_DIR) $(U_SERVICE_DOC).pdf

v14.vcxproj:
	@$(G_PYTHON) $(U_SPI_HOME)/makefiles/python$(G_PY_MAJOR_VERSION)/makeVcproj14Config.py $(U_VCXPROJ_OPTIONS) -b $(U_SPI_HOME)/build-win32/bin -t doc $(U_VCPROJ).v14.vcxproj $(U_VCPROJ) $(U_CONFIG_DIR) $(U_SERVICE_DOC).pdf

v15.vcxproj:
	@$(G_PYTHON) $(U_SPI_HOME)/makefiles/python$(G_PY_MAJOR_VERSION)/makeVcproj15Config.py $(U_VCXPROJ_OPTIONS) -b $(U_SPI_HOME)/build-win32/bin -t doc $(U_VCPROJ).v15.vcxproj $(U_VCPROJ) $(U_CONFIG_DIR) $(U_SERVICE_DOC).pdf

v16.vcxproj:
	@$(G_PYTHON) $(U_SPI_HOME)/makefiles/python$(G_PY_MAJOR_VERSION)/makeVcproj16Config.py $(U_VCXPROJ_OPTIONS) -b $(U_SPI_HOME)/build-win32/bin -t doc $(U_VCPROJ).v16.vcxproj $(U_VCPROJ) $(U_CONFIG_DIR) $(U_SERVICE_DOC).pdf

