default: build

U_SAMPLE_HOME=.
include $(U_SAMPLE_HOME)/config.mk

BUILD_DIRS=\
libs/isda-cds\
libs/isda-cds/errorHandler\
libs/isda-clib\
libs/cpplib/utils\
libs/cpplib/maths\
libs/cpplib/rates\
libs/qlLite\
product/clib/config\
product/clib/dll\
product/clib/doc\
product/clib/python\
product/clib/replay\
product/clib/excel\
product/clib/dotnet\
product/cpplib/config\
product/cpplib/doc\
product/cpplib/cpplib-spi\
product/cpplib/dll\
product/cpplib/python\
product/cpplib/replay\
product/cpplib/excel\
product/cpplib/dotnet\
product/ql/config\
product/ql/dll\
product/ql/doc\
product/ql/python\
product/ql/replay\
product/ql/excel\
product/ql/dotnet

TEST_CPP_DIRS=\
product/clib/test/cpp

TEST_DIRS=\
product/clib/test/regression\
product/cpplib/test/regression

build:
	@for lib in $(BUILD_DIRS); do \
		echo Building $$lib; \
		$(MAKE) -C $$lib; \
	done
	@for tdn in $(TEST_CPP_DIRS); do \
		echo Testing in $$tdn; \
		$(MAKE) -C $$tdn all; \
	done

clean:
	@for lib in $(BUILD_DIRS); do \
		$(MAKE) -C $$lib clean; \
	done

code:
	@for lib in $(BUILD_DIRS); do \
		if [ `basename $$lib` = "config" ]; then echo $$lib; $(MAKE) -s -C $$lib; fi \
	done

v15.vcxproj:
	@for lib in $(BUILD_DIRS) $(DOC_BUILD_DIRS) $(TEST_DIRS); do \
		echo Creating vcproject for $$lib; \
		$(MAKE) -s -C $$lib v15.vcxproj; \
	done

v16.vcxproj:
	@for lib in $(BUILD_DIRS) $(DOC_BUILD_DIRS) $(TEST_DIRS); do \
		echo Creating vcproject for $$lib; \
		$(MAKE) -s -C $$lib v16.vcxproj; \
	done

vc-all:
	$(MAKE) -s v15.vcxproj
	$(MAKE) -s v16.vcxproj

rebuild: clean build

test:
	@for dn in $(TEST_DIRS); do $(MAKE) all LOGGING=1 -C $$dn; done
	$(MAKE) test-summary

test-summary:
	@echo
	@echo "============================================"
	@echo "Showing summary results for regression tests"
	@echo "============================================"
	@for dn in $(TEST_DIRS); do $(MAKE) summary -C $$dn; done

