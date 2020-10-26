def printCashFlowList(fp, name, cfl):
    fp.write("CashFlowList (%s)\n" % name)
    dates = cfl.dates
    amounts = cfl.amounts
    numItems = cfl.numItems

    assert(len(dates) == numItems)
    assert(len(amounts) == numItems)

    fp.write("NumItems: %d\n" % numItems)
    if numItems > 0:
        fp.write("Date       Amount\n")
        fp.write("========== ==========\n")
        for date,amount in zip(dates,amounts):
            fp.write("%s %10.6f\n" % (date, amount))
