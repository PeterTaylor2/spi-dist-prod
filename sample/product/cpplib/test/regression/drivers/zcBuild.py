# tests for zero curve build functions
import os
import lib

from TestLib import TestDriver, TestMain

import cpplib as clib



class Driver(TestDriver):
    def __init__(self):
        print (clib.Version())

        TestDriver.__init__(self, [
            ("baseDate", TestDriver.DATE),
            ("cashDates", TestDriver.DATE, True),
            ("cashRates", TestDriver.FLOAT, True),
            ("cashDayCountConv", TestDriver.STRING),
            ("interpType", TestDriver.STRING),
            ("swapDates", TestDriver.DATE, True),
            ("swapRates", TestDriver.FLOAT, True),
            ("couponInterval", TestDriver.STRING),
            ("fixedDcc", TestDriver.STRING),
            ("badDayConv", TestDriver.STRING),
            ("holidays", TestDriver.STRING)])

    def runOne(self, fp,
        baseDate, cashDates, cashRates, cashDayCountConv, interpType,
        swapDates, swapRates, couponInterval, fixedDcc, badDayConv, holidays):

        zcCash = clib.ZeroCurveCash(baseDate, cashDates, cashRates,
            cashDayCountConv, interpType)
        zcSwaps = clib.ZeroCurveSwaps(zcCash, swapDates, swapRates,
            couponInterval, fixedDcc, badDayConv, holidays)

        fp.write("CashDate   CashRate   Recalc     Diff/bp\n")
        fp.write("========== ========== ========== =======\n")
        for cashDate,cashRate in zip(cashDates, cashRates):
            recalc = clib.MMRate(zcSwaps, baseDate, cashDate, cashDayCountConv)
            diff = abs(recalc - cashRate) * 1e4
            fp.write("%10s %9.6f%% %9.6f%% %7.5f\n" % (
                cashDate, 1e2*cashRate, 1e2*recalc, diff))

        fp.write("\n")
        fp.write("SwapDate   SwapRate   Recalc     Diff/bp\n")
        fp.write("========== ========== ========== =======\n")
        for swapDate,swapRate in zip(swapDates, swapRates):
            recalc = clib.SwapRate(zcSwaps, baseDate, couponInterval, swapDate,
                fixedDcc, badDayConv, badDayConv, holidays)
            diff = abs(recalc - swapRate) * 1e4
            fp.write("%10s %9.6f%% %9.6f%% %7.5f\n" % (
                swapDate, 1e2*swapRate, 1e2*recalc, diff))

if __name__ == "__main__": TestMain(Driver, __file__)


