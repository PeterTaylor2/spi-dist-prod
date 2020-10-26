PY_VERSION?=37
MEMCHECK?=0
DEBUG?=$(MEMCHECK)
WAIT?=0
PROFILE?=0

build:
	$(MAKE) -C $(U_PRODUCT_HOME)/python

build-all:
	$(MAKE) -C $(U_PRODUCT_HOME)/python all

CMP=$(G_PYTHON) $(U_SPI_HOME)/makefiles/regression-test/compareResults.py
ALL=$(G_PYTHON) $(U_SPI_HOME)/makefiles/regression-test/runAll.py
FIND_CALLS=$(G_PYTHON) $(U_SPI_HOME)/makefiles/regression-test/findCalls.py
MEMCHECKER=drmemory -show_reachable --

ifneq ($(MEMCHECK),0)
WRAPPER=$(MEMCHECKER)
endif

ifneq ($(WAIT),0)
OPTS+=-w
endif

ifneq ($(PROFILE),0)
PROFILE_OPTS+=--profile
endif

DRIVER:=$(basename $(notdir $(NAME)))

ABI_DIR:=$(U_PRODUCT_HOME)/$(G_ABI)/py$(PY_VERSION)
PYTHONPATH:=$(ABI_DIR);$(U_SPI_HOME)/makefiles/regression-test

PATH:=$(ABI_DIR):$(PATH)

export PYTHONPATH
export PATH

run-one:
ifndef NAME
	@echo "NAME is undefined"
	@exit 1
endif
	@echo ==================================================================
	@echo Running tests for $(NAME)
	@echo Writing to $(G_ABI)/$(DRIVER).out
	@echo PYTHONPATH="$(PYTHONPATH)"
	@echo ==================================================================
	@rm -f $(G_ABI)/$(DRIVER).out
	@mkdir -p $(G_ABI)
	@$(WRAPPER) $(G_PYTHON) $(NAME) $(OPTS) $(PROFILE_OPTS) inputs/$(DRIVER).inp $(G_ABI)/$(DRIVER).out
	@echo

run-all:
	@rm -f $(G_ABI)/*.out
	@rm -f $(G_ABI)/*.json
	@rm -f $(G_ABI)/*.diff
	@$(WRAPPER) $(ALL) $(OPTS) drivers inputs $(G_ABI)

compare-one:
ifndef NAME
	@echo "NAME is undefined"
	@exit 1
endif
	@echo ==================================================================
	@echo Comparing results for $(NAME)
	@echo Baseline: baseline/$(DRIVER).out
	@echo Results: $(G_ABI)/$(DRIVER).out
	@echo ==================================================================
	@$(CMP) baseline/$(DRIVER).out $(G_ABI)/$(DRIVER).out

compare-all:
	@rm -f $(G_ABI)/*.diff
	@$(CMP) --ext="out" baseline $(G_ABI)
	@echo
	@for fn in $(G_ABI)/*.diff; do\
		if [ -f $$fn ]; then echo "WARNING: $$fn"; fi; \
	done

clean:
	rm -fr $(G_ABI)

summary:
	@echo
	@echo ===========================================================================
	@echo Summary of regression test results
	@echo Directory: $(CURDIR)
	@echo Python: $(G_PYTHON)
	@echo ===========================================================================
	@$(G_PYTHON) $(U_SPI_HOME)/makefiles/regression-test/makeSummary.py $(G_ABI)
	@echo

detail:
	@echo
	@$(G_PYTHON) $(U_SPI_HOME)/makefiles/regression-test/makeSummary.py -d $(G_ABI)
	@echo
	@echo Tests run in $(CURDIR) using $(G_PYTHON)

v10.vcxproj:
	$(G_PYTHON) $(U_SPI_HOME)/makefiles/python$(G_PY_MAJOR_VERSION)/makeVcproj10Test.py -b $(U_SPI_HOME)/build-win32/bin $(U_TEST_NAME).v10.vcxproj $(U_TEST_NAME)

v14.vcxproj:
	$(G_PYTHON) $(U_SPI_HOME)/makefiles/python$(G_PY_MAJOR_VERSION)/makeVcproj14Test.py -b $(U_SPI_HOME)/build-win32/bin $(U_TEST_NAME).v14.vcxproj $(U_TEST_NAME)

v15.vcxproj:
	$(G_PYTHON) $(U_SPI_HOME)/makefiles/python$(G_PY_MAJOR_VERSION)/makeVcproj15Test.py -b $(U_SPI_HOME)/build-win32/bin $(U_TEST_NAME).v15.vcxproj $(U_TEST_NAME)

v16.vcxproj:
	$(G_PYTHON) $(U_SPI_HOME)/makefiles/python$(G_PY_MAJOR_VERSION)/makeVcproj16Test.py -b $(U_SPI_HOME)/build-win32/bin $(U_TEST_NAME).v16.vcxproj $(U_TEST_NAME)

