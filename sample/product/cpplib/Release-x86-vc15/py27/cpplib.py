"""
This is an example SPI library containing wrapper functions for an open
source C++ library.

Originally this was all implement via the ISDA/CDS C-library.

We have now implemented down to holidays.cfg in the new C++ library.

"""

import os

cwd = os.getcwd()
try:
    os.chdir(os.path.dirname(os.path.abspath(__file__)))
    import sys
    sys.path.insert(0, os.getcwd()) # needed by python3
    try:
        from py_cpplib import *
    finally: sys.path.pop(0)
finally: os.chdir(cwd)

