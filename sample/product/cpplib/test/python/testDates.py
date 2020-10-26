import unittest
import datetime

import py_clib as clib

class TestDates(unittest.TestCase):
    def setUp(self): pass
    def tearDown(self): pass
    def testDayOfWeek(self):
        """Tests clib.DayOfWeek function"""
        date = datetime.date(2014,10,4) # which is Saturday
        print "DayOfWeek(%s) = %d" % (date, clib.DayOfWeek(date))
        for dow in range(7):
            date += datetime.timedelta(1)
            self.assertEqual(clib.DayOfWeek(date), dow)
            pass #end-for
        pass #end-def
    def testDaysInMonth(self):
        """Tests clib.DaysInMonth function"""
        normalYear = 2014
        leapYear = 2016
        for month in range(1,13):
            normalDays = clib.DaysInMonth(normalYear, month)
            leapDays   = clib.DaysInMonth(leapYear, month)

            if month == 2: self.assertEqual(leapDays - normalDays, 1)
            else: self.assertEqual(leapDays, normalDays)

            if month in [1,3,5,7,8,10,12]:
                self.assertEqual(normalDays, 31)
            elif month in [4,6,9,11]:
                self.assertEqual(normalDays, 30)
            elif month == 2:
                self.assertEqual(normalDays, 28)
            else: raise Exception("PROGRAM BUG")
            pass #end-for
        pass #end-def
    def testDateToEOM(self):
        """Tests clib.DateToEOM and related functions"""
        for year in range(2015,2017):
            for month in range(1,13):
                daysInMonth = clib.DaysInMonth(year, month)
                date        = datetime.date(year, month, 15)
                endOfMonth  = clib.DateToEOM(date)
                self.assertEqual(endOfMonth, datetime.date(year, month, daysInMonth))
                pass #end-for
            pass #end-for
        pass #end-def
    def testDateIntervalFreq(self):
        """Tests clib.DateIntervalFreq"""
        # DateInterval is a simple type and we convert freely to/from strings
        frequencies = [12, 6, 4, 3, 2, 1]
        for frequency in frequencies:
            months = 12 / frequency
            interval = "%dM" % months
            freq = clib.DateIntervalFreq(interval)
            print interval, freq, frequency
            self.assertEqual(freq, frequency)
            pass #end-for
        pass #end-def
    def testDateIntervalYears(self):
        """Tests clib.DateIntervalYears"""
        # DateInterval is a simple type and we convert freely to/from strings
        years = [0.5, 1, 1.5, 2, 3, 4, 5]
        months = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12]
        weeks = [1, 2, 3]
        days = [1, 2, 3, 7, 14]

        for year in years:
            if year == float(int(year)):
                interval = "%dY" % int(year)
            else: interval = "%dM" % int(12 * year)
            result = clib.DateIntervalYears(interval)
            print interval, result
            self.assertEqual(result, year)
            pass #end-for

        for month in months:
            interval = "%dM" % month
            result = clib.DateIntervalYears(interval)
            print interval, result
            self.assertEqual(result, month/12.0)
            pass #end-for

        for week in weeks:
            interval = "%dW" % week
            result = clib.DateIntervalYears(interval)
            print interval, result
            self.assertEqual(result, 7*week/365.0)
            pass #end-for

        for day in days:
            interval = "%dD" % day
            result = clib.DateIntervalYears(interval)
            print interval, result
            self.assertEqual(result, day/365.0)
            pass #end-for
        pass #end-def
    pass #end-class

if __name__ == "__main__":
    clib.start_logging("dates.log")
    unittest.main()
    pass #end-if
    
