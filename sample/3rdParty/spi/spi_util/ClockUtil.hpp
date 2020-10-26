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
#ifndef SPI_CLOCK_UTIL_HPP
#define SPI_CLOCK_UTIL_HPP

/**
***************************************************************************
** ClockUtil.hpp
***************************************************************************
** Low level functions for examining the system clock for the purpose
** of accurate timing of functions.
***************************************************************************
*/

#include "DeclSpec.h"
#include "Namespace.hpp"

SPI_UTIL_NAMESPACE

SPI_UTIL_IMPORT
void Sleep(int ms);

class SPI_UTIL_IMPORT Clock
{
public:
    Clock();
    void Start();
    double Time();
private:
    double m_start;
    static double g_scaling;
};

/*
 * For low-level logging purposes call ClockEventsStart to start keeping track
 * of time.
 *
 * When you have an event that you want to log then call ClockEventsLog.
 *
 * When you are done call ClockEventsWrite which will flush the entries to file.
 */
SPI_UTIL_IMPORT
void ClockEventsStart();

/*
 * Call this to log an event after you have called ClockEventsStart.
 * If ClockEventsStart has not been called then this function does nothing.
 * Note that msg must be static data as it is only consumed later.
 */
SPI_UTIL_IMPORT
void ClockEventsLog(const char* msg);

/*
 * Call this to write all the events to the given file.
 * If ClockEventsStart has not been called then this function does nothing.
 */
SPI_UTIL_IMPORT
void ClockEventsWrite(const char* filename);

/*
 * If you want to clock a function then create a ClockFunction object at the
 * start of the function.
 *
 * When the function exits the ClockFunction will go out of scope and it will
 * log the time spent in the function.
 */
class SPI_UTIL_IMPORT ClockFunction
{
public:
    ClockFunction(const char* func);
    ~ClockFunction();
private:
    const char* m_func;
    double m_unallocatedTime;
};

SPI_UTIL_END_NAMESPACE

#ifdef SPI_UTIL_CLOCK_EVENTS

#define SPI_UTIL_CLOCK_EVENTS_START() spi_util::ClockEventsStart()
#define SPI_UTIL_CLOCK_EVENTS_LOG(msg) spi_util::ClockEventsLog(msg)
#define SPI_UTIL_CLOCK_EVENTS_WRITE(filename) spi_util::ClockEventsWrite(filename, true)
#define SPI_UTIL_CLOCK_EVENTS_WRITE_DETAILS(filename) spi_util::ClockEventsWrite(filename, false)
#define SPI_UTIL_CLOCK_FUNCTION() spi_util::ClockFunction clock_function_(__FUNCTION__)
#define SPI_UTIL_CLOCK_BLOCK(name) spi_util::ClockFunction clock_block_(name)

#else

#define SPI_UTIL_CLOCK_EVENTS_START()
#define SPI_UTIL_CLOCK_EVENTS_LOG(msg)
#define SPI_UTIL_CLOCK_EVENTS_WRITE(filename)
#define SPI_UTIL_CLOCK_EVENTS_WRITE_DETAILS(filename)
#define SPI_UTIL_CLOCK_FUNCTION()
#define SPI_UTIL_CLOCK_BLOCK(name)

#endif

#endif
