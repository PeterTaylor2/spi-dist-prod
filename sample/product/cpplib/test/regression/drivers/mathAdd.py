import lib

from TestLib import TestDriver, TestMain

import cpplib as clib

class Driver(TestDriver):
    def __init__(self):
        print (clib.Version())
        TestDriver.__init__(self, [
            ("x", TestDriver.FLOAT),
            ("y", TestDriver.FLOAT),
            ("z", TestDriver.FLOAT)])

    def runOne(self, fp, x, y, z):
        fp.write("Result: %12.6f\n" % (clib.Math.Add(x,y,z)))

if __name__ == "__main__": TestMain(Driver, __file__)


