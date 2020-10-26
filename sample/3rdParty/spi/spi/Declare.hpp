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
** Declare.hpp
***************************************************************************
** Defines shared pointer types for non reference counted classes.
***************************************************************************
*/

#ifndef SPI_DECLARE_HPP
#define SPI_DECLARE_HPP

#include "platform.h"
#include "DeclSpec.h"
#include "Namespace.hpp"

#include <stdexcept>

#include <spi_boost/shared_ptr.hpp>

#undef SPI_DECLARE_CLASS
#define SPI_DECLARE_CLASS(T) \
class T;\
typedef spi_boost::shared_ptr<T>       T##SP;\
typedef spi_boost::shared_ptr<T const> T##ConstSP

#undef SPI_DECLARE_STRUCT
#define SPI_DECLARE_STRUCT(T) \
struct T;\
typedef spi_boost::shared_ptr<T>       T##SP;\
typedef spi_boost::shared_ptr<T const> T##ConstSP

#endif


