import ql

import datetime
import pprint

print ("")
print ("calling Add")
print (ql.Add(2.0, 5.0))

print ("")
print ("calling DateToString")

dates = []
for month,day in zip(
    [1,2,3,4,5,6,7,8,9,10,11,12],
    [15,14,13,12,11,10,9,8,7,6,5,4]):

    dt = datetime.date(2013,month,day)
    print ("%s %s" % (dt, ql.DateToString(dt)))
    dates.append(dt)

print ("")
print ("testing array formula for DateToString")
pprint.pprint (ql.DateToString(dates))

