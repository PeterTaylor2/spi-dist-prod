"""
This is an example SPI library containing wrapper functions for the
ISDA/CDS C-library.

In general we will not be using C++ except for error handling, hence the
name CLIB (C-library)

"""

import os

cwd = os.getcwd()
try:
    os.chdir(os.path.dirname(os.path.abspath(__file__)))
    import sys
    sys.path.insert(0, os.getcwd()) # needed by python3
    try:
        from py_clib import *
    finally: sys.path.pop(0)
finally: os.chdir(cwd)

