default: build

U_SAMPLE_HOME=..
include $(U_SAMPLE_HOME)/config.mk

BUILD_DIRS=\
isda-cds\
isda-cds/errorHandler\
isda-clib\
qlLite\
cpplib/utils\
cpplib/maths\
cpplib/rates

build:
	@for lib in $(BUILD_DIRS); do \
		echo Building $$lib; \
		$(MAKE) -C $$lib; \
	done

clean:
	@for lib in $(BUILD_DIRS); do \
		$(MAKE) -C $$lib clean; \
	done

doc:
	@for doc in $(DOC_BUILD_DIRS); do \
		echo Building $$doc; \
		$(MAKE) -C $$doc; \
	done

vcproj:
	@for lib in $(BUILD_DIRS) $(DOC_BUILD_DIRS); do \
		echo Creating vcproject for $$lib; \
		$(MAKE) -s -C $$lib vcproj; \
	done

v14.vcxproj:
	@for lib in $(BUILD_DIRS) $(DOC_BUILD_DIRS); do \
		echo Creating vcproject for $$lib; \
		$(MAKE) -s -C $$lib v14.vcxproj; \
	done

v15.vcxproj:
	@for lib in $(BUILD_DIRS) $(DOC_BUILD_DIRS); do \
		echo Creating vcproject for $$lib; \
		$(MAKE) -s -C $$lib v15.vcxproj; \
	done

v16.vcxproj:
	@for lib in $(BUILD_DIRS) $(DOC_BUILD_DIRS); do \
		echo Creating vcproject for $$lib; \
		$(MAKE) -s -C $$lib v16.vcxproj; \
	done

rebuild: clean build

