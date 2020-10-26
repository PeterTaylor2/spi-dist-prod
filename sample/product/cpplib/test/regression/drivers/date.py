# tests for stand-alone date functions

import lib

from TestLib import TestDriver, TestMain

import cpplib as clib

class Driver(TestDriver):
    def __init__(self):
        print (clib.Version())
        TestDriver.__init__(self, [
            ("date", TestDriver.DATE)])

    def runOne(self, fp, date):
        fp.write("DayOfWeek:     %s\n" % (clib.DayOfWeek(date)))
        fp.write("EndOfMonth:    %s\n" % (clib.DateToEOM(date)))
        year,month,day = clib.YMD(date)
        fp.write("YMD:           %d,%d,%d\n" % (year,month,day))
        assert(year == clib.Year(date))
        fp.write("StartOfYear:   %s\n" % (clib.StartOfYear(year)))
        startOfMonths = clib.StartOfMonths(year)
        fp.write("StartOfMonths: %s\n" % (", ".join(
            [str(dt) for dt in startOfMonths])))
        fp.write("DaysInMonth:   %d\n" % (clib.DaysInMonth(year,month)))

if __name__ == "__main__": TestMain(Driver, __file__)


