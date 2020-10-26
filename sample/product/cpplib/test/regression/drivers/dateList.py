# tests for date list functions

import lib

from TestLib import TestDriver, TestMain

import cpplib as clib

def printDateList(fp, name, dl):
    fp.write("DateList (%s)\n" % name)
    if dl is None:
        fp.write("None\n")
        fp.write("\n")
        return
    fp.write("NumItems: %d\n" % dl.numItems)
    dates = dl.dates
    assert(len(dates) == dl.numItems)
    for i in range(len(dates)):
        fp.write("%2d: %s\n" % (i+1, dates[i]))
    fp.write("\n")

class Driver(TestDriver):
    def __init__(self):
        print (clib.Version())
        TestDriver.__init__(self, [
            ("dates", TestDriver.DATE, True),
            ("addDates", TestDriver.DATE, True)])

    def runOne(self, fp, dates, addDates):

        dl = clib.DateList(dates) if len(dates) else None
        dl2 = clib.DateList(addDates) if len(addDates) else None
        dl3 = clib.DateListMerge(dl, dl2)

        printDateList(fp, "dates", dl)
        printDateList(fp, "addDates", dl2)
        printDateList(fp, "mergeDates", dl3)

if __name__ == "__main__": TestMain(Driver, __file__)


