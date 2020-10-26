build:

U_SAMPLE_HOME=../..
include $(U_SAMPLE_HOME)/config.mk

BUILD_DIRS=\
../../libs/isda-cds\
../../libs/isda-cds/errorHandler\
../../libs/isda-clib\
config\
dll\
doc\
python\
replay

ifeq ($(G_PLATFORM),win32)
BUILD_DIRS+=excel dotnet 
endif

build:
	@for lib in $(BUILD_DIRS); do \
		echo Building $$lib; \
		$(MAKE) -C $$lib; \
	done

clean:
	@for lib in $(BUILD_DIRS); do \
		$(MAKE) -C $$lib clean; \
	done

vcproj:
	@for lib in $(BUILD_DIRS); do \
		echo Creating vcproject for $$lib; \
		$(MAKE) -s -C $$lib vcproj; \
	done

vcxproj:
	@for lib in $(BUILD_DIRS); do \
		echo Creating vcproject for $$lib; \
		$(MAKE) -s -C $$lib vcxproj; \
	done

rebuild: clean build

spi:
	$(MAKE) -C $(U_SPI_HOME)

test:
	@rm -f replay/rates.out
	$(MAKE) -C replay run ARGS="rates.log rates.out"
	cd replay; diff rates.log rates.out
