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
            ("dayCountConv", TestDriver.STRING),
            ("rateType", TestDriver.STRING)])

    def runOne(self, fp,
        baseDate, baseRate, slope, hump, interpType,
        startDate, maturityDate, dayCountConv, rateType):

        zc = clib.ZeroCurveSample(baseDate, baseRate, slope, hump, interpType)
        zeroRate = zc.ZeroRate(maturityDate)
        pv = zc.PV(maturityDate)
        fv = zc.FV(startDate, maturityDate)
        fwdRate = clib.ForwardRate(zc, startDate, maturityDate, dayCountConv, rateType)
        mmRate = clib.MMRate(zc, startDate, maturityDate, dayCountConv)

        fp.write("zeroRate: %10.6f%%\n" % (1e2*zeroRate))
        fp.write("pv:       %10.6f\n" % pv)
        fp.write("fv:       %10.6f\n" % fv)
        fp.write("fwdRate:  %10.6f%%\n" % (1e2*fwdRate))
        fp.write("mmRate:   %10.6f%%\n" % (1e2*mmRate))

if __name__ == "__main__": TestMain(Driver, __file__)


