using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using sartorial;

namespace test_sample
{
    using clib = sartorial.clib;

    class Program
    {
        static void Main(string[] args)
        {
            clib.clib_start_service();
            //clib.clib_start_logging("clib.log", "");

            Console.Write("Enter to continue:");
            Console.ReadLine();
            string version = clib.Version(false);
            Console.WriteLine(version);
            double x = clib.Math.Add(1, 2, 3);
            double y = clib.Math.Add(2, 2);
            double z = clib.Math.BinaryOp(4.3, clib.Math.BinaryOperator.Subtract, 1.3);

            DateTime aDate = clib.Date(2014, 12, 12);

            DateTime startDate = new DateTime(2014, 3, 15);
            DateTime endDate = new DateTime(2014, 4, 15);

            clib.DayCountConv dcc = "ACT/365F";
            double time = clib.DayCountFraction(startDate, endDate, dcc);
            int days = clib.DaysDiff(startDate, endDate, dcc);

            DateTime addYear = clib.AddDateInterval(startDate, "1Y");
            DateTime[] dates = new DateTime[5];
            double[] rates = new double[5];
            double[] amounts = new double[5];

            DateTime lastDate = startDate;
            for (int i = 0; i < 5; ++i)
            {
                rates[i] = (i + 1) / 1e2;
                dates[i] = lastDate;
                amounts[i] = 1.0;

                lastDate = clib.AddDateInterval(lastDate, "3M");
            }

            clib.CashFlowList cfl = new clib.CashFlowList(dates, amounts);

            string cflString = cfl.to_string();
            
            clib.CashFlowList cfl2 = clib.CashFlowList.from_string(cflString);
            int numItems = cfl2.get_value("numItems"); // implicit type conversion in action
            DateTime[] cflDates = cfl2.get_value("dates");

            clib.ZeroCurve zc = new clib.ZeroCurve(
                startDate, clib.InterpType.FLAT_FORWARDS, dates, rates, "Annual", "ACT/365F");

            Console.WriteLine(zc.to_string());

            double discount1 = zc.PV(startDate);
            double discount2 = zc.PV(endDate);
            double discount3 = zc.PV(endDate); 

            Console.WriteLine("endDate {1} discountFactor {0}", discount2, endDate);

            clib.DateOrInterval doiDate = new System.DateTime(2018, 5, 15);
            Console.WriteLine(doiDate.to_string());
            clib.DateOrInterval doiIVL = "3M";
            Console.WriteLine(doiIVL.to_string());

            clib.DateOrInterval doi2 = new clib.DateOrIntervalDate(endDate, clib.BadDayConv.MODIFIED);
            clib.DateOrIntervalDate doi3 = new clib.DateOrIntervalDate(endDate, clib.BadDayConv.MODIFIED);

            //startDate = clib.Date(2014, 9, 15);
            //Variant[] offsets = new Variant[3];
            //offsets[0] = new Variant("1M");
            //offsets[1] = new Variant(clib.Date(2014,11,15));
            //offsets[2] = new Variant("6M");

            //System.DateTime[] endDates = clib.DateFwdAdjust(startDate, offsets);

            double[,] matrixData = new double[2, 3];

            matrixData[0, 0] = 1.0;
            matrixData[0, 1] = 2.0;
            matrixData[0, 2] = 3.0;
            matrixData[1, 0] = 1.7;
            matrixData[1, 1] = 2.5;
            matrixData[1, 2] = 3.2;

            clib.Math.Matrix mat = new clib.Math.Matrix(matrixData);
            var size = mat.Size();

            Console.WriteLine(mat.to_string());
            Console.WriteLine("cols = {0}; rows = {1}", size.cols, size.rows);

            double[,] xxx = mat.data;
            double[,] xxxReturned = mat.Data();

            Console.WriteLine(clib.Version());
        }
    }
}
