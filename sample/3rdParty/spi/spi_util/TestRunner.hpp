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
#ifndef SPI_UTIL_TESTRUNNER_HPP
#define SPI_UTIL_TESTRUNNER_HPP

#include "DeclSpec.h"
#include "Namespace.hpp"

SPI_UTIL_NAMESPACE

typedef void (TestRoutine)(void);

/**
 * Runs a number of tests with controls from the command line.
 * You define the tests in pairs with name (const char*) followed by
 * test routine (function with no inputs and no outputs).
 *
 * The options are as follows:
 *
 * -w:  Wait for input before proceeding with each test.
 *      This gives you a chance to attach a debugger.
 * -rN: Repeat tests N times (for timing purposes).
 * -t:  Run timings (future enhancement).
 *
 * If you provide no command line arguments, then all tests will be run.
 * Otherwise any command line arguments indicate the tests that you want
 * to run specifically.
 *
 * Returns the number of errors detected which can be used as the
 * return value from the test routine.
 *
 * The end of the list of arguments should be indicated by passing (char*)0
 * (not simply 0 since that will be confusing in 64-bit environment).
 */
SPI_UTIL_IMPORT
int TestRunner(int argc, char* argv[], ...);

SPI_UTIL_END_NAMESPACE

#endif // SPI_UTIL_TESTRUNNER_HPP
