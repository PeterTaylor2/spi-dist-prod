###########################################################################
# Designed to make Visual C++ project files.
#
# Requires U_TARGET and U_SPI_HOME for the main library builds.
# Requires U_SRC_DIR and U_INC_DIR for the VC project build.
# Optionally uses U_VCPROJ_OPTIONS for the VC project build.
#
# You would need U_VCPROJ_OPTIONS if there are files in the U_SRC_DIR
# that you do not want to appear in your project file.
###########################################################################

I_VCPROJ9_CMD=$(G_PYTHON) $(U_SPI_HOME)/makefiles/python$(G_PY_MAJOR_VERSION)/makeVcproj9.py
I_VCPROJ10_CMD=$(G_PYTHON) $(U_SPI_HOME)/makefiles/python$(G_PY_MAJOR_VERSION)/makeVcproj10.py
I_VCPROJ14_CMD=$(G_PYTHON) $(U_SPI_HOME)/makefiles/python$(G_PY_MAJOR_VERSION)/makeVcproj14.py
I_VCPROJ15_CMD=$(G_PYTHON) $(U_SPI_HOME)/makefiles/python$(G_PY_MAJOR_VERSION)/makeVcproj15.py
I_VCPROJ16_CMD=$(G_PYTHON) $(U_SPI_HOME)/makefiles/python$(G_PY_MAJOR_VERSION)/makeVcproj16.py

ifndef U_VCPROJ
I_VCPROJ=$(U_TARGET)
else
I_VCPROJ=$(U_VCPROJ)
endif

vcproj:: 
	@$(I_VCPROJ9_CMD) $(U_VCPROJ_OPTIONS) $(I_VCPROJ).vcproj $(I_VCPROJ) $(U_SRC_DIR) $(U_INC_DIR) $(notdir $(I_TARGET))

v10.vcxproj:: 
	@$(I_VCPROJ10_CMD) $(I_INCLUDES) $(U_VCPROJ_OPTIONS) -b $(U_SPI_HOME)/build-win32/bin $(I_VCPROJ).v10.vcxproj $(I_VCPROJ) $(U_SRC_DIR) $(U_INC_DIR)

v14.vcxproj:: 
	@$(I_VCPROJ14_CMD) $(I_INCLUDES) $(U_VCPROJ_OPTIONS) -b $(U_SPI_HOME)/build-win32/bin $(I_VCPROJ).v14.vcxproj $(I_VCPROJ) $(U_SRC_DIR) $(U_INC_DIR)

v15.vcxproj:: 
	@$(I_VCPROJ15_CMD) $(I_INCLUDES) $(U_VCPROJ_OPTIONS) -b $(U_SPI_HOME)/build-win32/bin $(I_VCPROJ).v15.vcxproj $(I_VCPROJ) $(U_SRC_DIR) $(U_INC_DIR)

v16.vcxproj:: 
	@$(I_VCPROJ16_CMD) $(I_INCLUDES) $(U_VCPROJ_OPTIONS) -b $(U_SPI_HOME)/build-win32/bin $(I_VCPROJ).v16.vcxproj $(I_VCPROJ) $(U_SRC_DIR) $(U_INC_DIR)

