# tests for day count convention functions

import lib

from TestLib import TestDriver, TestMain

import clib

class Driver(TestDriver):
    def __init__(self):
        print (clib.Version())
        TestDriver.__init__(self, [
            ("startDate", TestDriver.DATE),
            ("endDate", TestDriver.DATE),
            ("dayCountConv", TestDriver.STRING)])

    def runOne(self, fp, startDate, endDate, dayCountConv):

        try:
            dccObject = clib.DayCountConv.Coerce(dayCountConv)
        except Exception as e:
            fp.write("ERROR: %s\n" % str(e))
            return

        fp.write("DayCountFraction: %8.6f\n" % (clib.DayCountFraction(
            startDate, endDate, dayCountConv)))
        fp.write("DaysDiff:         %d\n" % (clib.DaysDiff(
            startDate, endDate, dayCountConv)))

if __name__ == "__main__": TestMain(Driver, __file__)


