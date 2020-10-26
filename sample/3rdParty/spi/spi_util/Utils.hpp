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
/*
***************************************************************************
** Utils.hpp
***************************************************************************
** Various utilities - mostly simple templates.
***************************************************************************
*/

#ifndef SPI_UTIL_UTILS_HPP
#define SPI_UTIL_UTILS_HPP

#include "DeclSpec.h"
#include "Namespace.hpp"
#include "RuntimeError.hpp"

SPI_UTIL_NAMESPACE

template <typename T, typename U>
T IntegerCast(U in)
{
    T out = (T)in;
    if ((U)out != in)
        SPI_UTIL_THROW_RUNTIME_ERROR(in << " is out of range");
    return out;
}

SPI_UTIL_IMPORT
std::string Timestamp();

SPI_UTIL_END_NAMESPACE

#endif
