"""
This is a description of the entire service. As such it will appear
at the start of the generated manual when we get to the point of
generating manuals.

You should be describing the business functionality of the service.

Descriptions can be in multiple blocks for the same object being
described, and they will be merged together in such cases.
A new description will create a new paragraph.

"""

import os

cwd = os.getcwd()
try:
    os.chdir(os.path.dirname(os.path.abspath(__file__)))
    import sys
    sys.path.insert(0, os.getcwd()) # needed by python3
    try:
        from py_ql import *
    finally: sys.path.pop(0)
finally: os.chdir(cwd)

