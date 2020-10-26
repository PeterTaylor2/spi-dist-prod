import lib

from TestLib import TestDriver, TestMain

import clib

class Driver(TestDriver):
    def __init__(self):
        print (clib.Version())
        TestDriver.__init__(self, [
            ("x",        TestDriver.FLOAT),
            ("binaryOp", TestDriver.STRING),
            ("y",        TestDriver.FLOAT)])

    def runOne(self, fp, x, binaryOp, y):
        result = clib.Math.BinaryOp(x, binaryOp, y)
        fp.write("Result: %12.6f\n" % result)

if __name__ == "__main__": TestMain(Driver, __file__)


