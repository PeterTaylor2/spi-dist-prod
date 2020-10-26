/*

Copyright (C) 2012-2019 Sartorial Programming Ltd.

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

*/
/**
***************************************************************************
** Date.hpp
***************************************************************************
** Date class which internally uses the number of days since some arbitrary
** base date (1601-01-01 in the Gregorian calendar).
***************************************************************************
*/

#ifndef SPI_DATE_HPP
#define SPI_DATE_HPP

#include "DeclSpec.h"
#include "Namespace.hpp"

#include <string>

SPI_BEGIN_NAMESPACE

/**
 * Date class contains no virtual functions - hence the size of the class
 * is exactly the same as the size of its data fields - hence we go for
 * minimality, so that a vector of Date is as cheap as possible (for
 * example).
 */
class SPI_IMPORT Date
{
public:
    // constructors
    Date();
    Date(int dt);
    Date(long dt);
    Date(int year, int month, int day);
    static Date FromString(const std::string& str);
    static Date Today();

    // access methods
    void YMD(int* year, int* month, int* day) const;
    int Year() const;
    int Month() const;
    int Day() const;
    operator int() const;
    operator long() const;
    operator bool() const;
    std::string ToString() const;
    operator std::string() const;

    // in addition we will need functions which act on dates
    // e.g. add a number of days, months (pay attention to end of month
    //      adjustment rules), years (as above), subtract two dates to
    //      give a number of days
    //
    // most of these should be outside the class - e.g. operator + and
    // operator - for adding days or getting the number of days between
    // two dates

    Date& operator += (int days);
    Date& operator -= (int days);

private:
    int m_dt;
};

// simple Date arithmetic
// it is possible that we don't really need all the comparison methods here
// however it should help to avoid ambiguity

inline bool operator == (Date d1, Date d2)
{
    return (int)d1 == (int)d2;
}

inline bool operator != (Date d1, Date d2)
{
    return (int)d1 != (int)d2;
}

inline bool operator < (Date d1, Date d2)
{
    return (int)d1 < (int)d2;
}

inline bool operator > (Date d1, Date d2)
{
    return (int)d1 > (int)d2;
}

inline bool operator <= (Date d1, Date d2)
{
    return (int)d1 <= (int)d2;
}

inline bool operator >= (Date d1, Date d2)
{
    return (int)d1 >= (int)d2;
}

// difference between two dates is returned as an integer number of days

inline int operator - (Date d1, Date d2)
{
    return (int)d1 - (int)d2;
}

// we can a number of days and get another date

inline Date operator + (Date dt, int days)
{
    return Date((int)dt + days);
}

// we can subtract a number of days and get another date

inline Date operator - (Date dt, int days)
{
    return Date((int)dt - days);
}

SPI_END_NAMESPACE

#endif


