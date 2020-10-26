# tests for date interval functions

import lib

from TestLib import TestDriver, TestMain

import cpplib as clib

class Driver(TestDriver):
    def __init__(self):
        print (clib.Version())
        TestDriver.__init__(self, [
            ("date", TestDriver.DATE),
            ("dateInterval", TestDriver.STRING)])

    def runOne(self, fp, date, dateInterval):

        try:
            ivlObject = clib.DateInterval.Coerce(dateInterval)
        except Exception as e:
            fp.write("%s\n" % str(e))
            return

        fp.write("DateIntervalYears:   %8.6f\n" % (
            clib.DateIntervalYears(dateInterval)))

        fp.write("DateIntervalFreq:    %8.6f\n" % (
            clib.DateIntervalFreq(dateInterval)))

        fp.write("AddDateInterval(1):  %s\n" % (
            clib.AddDateInterval(date, dateInterval)))

        fp.write("AddDateInterval(4):  %s\n" % (
            clib.AddDateInterval(date, dateInterval, 4)))

        fp.write("AddDateInterval(0):  %s\n" % (
            clib.AddDateInterval(date, dateInterval, 0)))

        fp.write("AddDateInterval(-1): %s\n" % (
            clib.AddDateInterval(date, dateInterval, -1)))

if __name__ == "__main__": TestMain(Driver, __file__)


