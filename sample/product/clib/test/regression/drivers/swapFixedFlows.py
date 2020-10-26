# tests for cash flow generation functions
import os
import lib

from TestLib import TestDriver, TestMain

import clib

class Driver(TestDriver):
    def __init__(self):
        print (clib.Version())
        self.dnHolidays = os.path.abspath(
            os.path.join(
                os.path.dirname(__file__),
                "..", "..", "..", "..", "..", "data", "Holiday"))
        TestDriver.__init__(self, [
            ("couponRate", TestDriver.FLOAT),
            ("startDate", TestDriver.DATE),
            ("couponInterval", TestDriver.STRING),
            ("maturityDate", TestDriver.DATE),
            ("dayCountConv", TestDriver.STRING),
            ("frontStub", TestDriver.BOOL),
            ("shortStub", TestDriver.BOOL),
            ("subtractInitial", TestDriver.BOOL),
            ("keepStartDate", TestDriver.BOOL),
            ("addFinal", TestDriver.BOOL),
            ("accBadDayConv", TestDriver.STRING),
            ("payBadDayConv", TestDriver.STRING),
            ("holidays", TestDriver.STRING)])

    def runOne(self, fp,
        couponRate, startDate, couponInterval, maturityDate, dayCountConv,
        frontStub, shortStub, subtractInitial, keepStartDate, addFinal,
        accBadDayConv, payBadDayConv, holidays):

        if os.path.isfile(os.path.join(self.dnHolidays, holidays)):
            holidays = os.path.join(self.dnHolidays, holidays)

        fixedFlows = clib.SwapFixedFlows(
            couponRate, startDate, couponInterval, maturityDate, dayCountConv,
            frontStub, shortStub, subtractInitial, keepStartDate, addFinal,
            accBadDayConv, payBadDayConv, holidays)

        lib.printCashFlowList(fp, "fixedFlows", fixedFlows)

if __name__ == "__main__": TestMain(Driver, __file__)


