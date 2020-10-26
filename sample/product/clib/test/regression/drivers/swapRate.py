# tests for zero curve functions
import os
import lib

from TestLib import TestDriver, TestMain

import clib

class Driver(TestDriver):
    def __init__(self):
        print (clib.Version())
        TestDriver.__init__(self, [
            ("baseDate", TestDriver.DATE),
            ("baseRate", TestDriver.FLOAT),
            ("slope", TestDriver.FLOAT),
            ("hump", TestDriver.FLOAT),
            ("interpType", TestDriver.STRING),
            ("startDate", TestDriver.DATE),
            ("maturityDate", TestDriver.DATE),
            ("couponInterval", TestDriver.STRING),
            ("dayCountConv", TestDriver.STRING),
            ("badDayConv", TestDriver.STRING),
            ("holidays", TestDriver.STRING),
            ("stubAtEnd", TestDriver.BOOL)])

    def runOne(self, fp,
        baseDate, baseRate, slope, hump, interpType,
        startDate, maturityDate, couponInterval, dayCountConv,
        badDayConv, holidays, stubAtEnd):

        zc = clib.ZeroCurveSample(baseDate, baseRate, slope, hump, interpType)
        swapRate = clib.SwapRate(zc, startDate, couponInterval,
            maturityDate, dayCountConv, badDayConv, badDayConv, holidays,
            stubAtEnd)

        fp.write("swapRate: %10.6f%%\n" % (1e2*swapRate))

if __name__ == "__main__": TestMain(Driver, __file__)


