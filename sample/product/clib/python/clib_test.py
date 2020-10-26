import clib

import datetime
import pickle

input("continue:")

print (clib.Version())

print ("calling clib.Math.Add")
print (clib.Math.Add(2.0, 5.0))

print ("calling AddDateInterval")
startDate = datetime.date(2013,7,15)
for ivl in "3M,6M,1Y,2Y".split(","):
    print ("%s %s %s" % (startDate, ivl, clib.AddDateInterval(startDate, ivl)))

print ("calling DayCountFraction")
startDate = datetime.date(2012,6,15)
endDate = datetime.date(2012,12,15)
for dcc in ["Actual/365", "Actual/365F", "Actual/360", "30/360", "30E/360",
            "Effective"]:
    print ("%s %s %s %.17g" % (startDate, endDate, dcc, 
            clib.DayCountFraction(startDate, endDate, dcc)))

for diff in range(10):
    date = startDate + datetime.timedelta(diff)
    print ("%s %s" % (date, clib.DayOfWeek(date)))

print ("calling ZeroCurve constructor")
baseDate = datetime.date(2013,6,10)
dates = [datetime.date(2013,7,10), datetime.date(2014,6,10), datetime.date(2018,6,10)]
rates = [0.01, 0.02, 0.03]

interpType = "FLAT_FORWARDS"
rateType = "Annual"
curve = clib.ZeroCurve(baseDate, interpType, dates, rates, rateType, "ACT/365F")
print (curve.to_string())
dayCount = "Actual/365f"
curve = clib.ZeroCurve(baseDate, interpType, dates, rates, rateType, dayCount)
print (curve.to_string())
print ("hasattr(curve,'meta_data') %s" % hasattr(curve,'meta_data'))

for date in dates:
    discount = curve.PV(date)
    print ("%s %.17g" % (date, discount))

s = curve.to_string("", "ACC", 
    {"edition" : 1,
     "timestamp" : datetime.datetime.now()})

print (s)
z = clib.ZeroCurve.from_string(s)
print ("hasattr(z,'meta_data') %s" % hasattr(z,'meta_data'))
print (z.meta_data.edition)
print (z.meta_data.timestamp)
                               "ACT/360", interpType)

print ("CashCurve:\n%s" % cashCurve.to_string())
    print ("%s %.12g" % (
            date, 
            clib.ForwardRate(swapCurve, baseDate, date, "ACT/360", "SIMPLE")))

s = curve.to_string("JSON", "", 
    {"edition" : 1,
     "datestamp" : datetime.date.today()})
print (s)
clib.ZeroCurve.from_string(s)

x = pickle.dumps(curve)
y = pickle.loads(x)

print (x)
print (y.to_string("", "ACC"))

