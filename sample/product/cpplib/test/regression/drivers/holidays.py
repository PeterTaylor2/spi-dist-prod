# tests for stand-alone date functions
import os
import lib

from TestLib import TestDriver, TestMain

import cpplib as clib

class Driver(TestDriver):
    def __init__(self):
        print (clib.Version())
        self.dnHolidays = os.path.abspath(
            os.path.join(
                os.path.dirname(__file__),
                "..", "..", "..", "..", "..", "data", "Holiday"))
        TestDriver.__init__(self, [
            ("holidays", TestDriver.STRING),
            ("startDate", TestDriver.DATE),
            ("endDate", TestDriver.DATE),
            ("offset", TestDriver.INT),
            ("badDayConv", TestDriver.STRING),
            ("interval", TestDriver.STRING),
            ("adjType", TestDriver.STRING),
            ("dateOrInterval", TestDriver.STRING)])

    def runOne(self, fp,
        holidays, startDate, endDate, offset, badDayConv, interval, adjType,
        dateOrInterval):

        if os.path.isfile(os.path.join(self.dnHolidays, holidays)):
            holidays = os.path.join(self.dnHolidays, holidays)
            pass #end-if
        dates = clib.HolidaysGetDates(holidays)
        isBusinessDay = clib.IsBusinessDay(startDate, holidays)
        businessDay = clib.BusinessDay(startDate, badDayConv, holidays)
        isHoliday = clib.IsHoliday(startDate, holidays)
        isWeekend = clib.IsWeekend(startDate)
        busDaysDiff = clib.BusDaysDiff(startDate, endDate, holidays)
        addBusDays = clib.AddBusDays(startDate, offset, holidays)
        dateFwdBus = clib.DateFwdBus(startDate, 1, interval, badDayConv, holidays)
        dateFwdAdj = clib.DateFwdAdj(startDate, offset, adjType, holidays)
        doiEndDate = clib.DOIEndDate(startDate, dateOrInterval, holidays)
        businessEOM = clib.BusinessEOM(startDate, holidays)

        fp.write("NumberOfDates: %d\n" % len(dates))
        fp.write("IsBusinessDay: %s\n" % isBusinessDay)
        fp.write("IsHoliday:     %s\n" % isHoliday)
        fp.write("IsWeekend:     %s\n" % isWeekend)
        fp.write("BusDaysDiff:   %d\n" % busDaysDiff)
        fp.write("BusinessDay:   %s\n" % businessDay)
        fp.write("AddBusDays:    %s\n" % addBusDays)
        fp.write("DateFwdBus:    %s\n" % dateFwdBus)
        fp.write("DateFwdAdj:    %s\n" % dateFwdAdj)
        fp.write("DOIEndDate:    %s\n" % doiEndDate)
        fp.write("BusinessEOM:   %s\n" % businessEOM)

if __name__ == "__main__": TestMain(Driver, __file__)


