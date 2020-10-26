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
** Namespace.hpp
***************************************************************************
** Defines the namespace macros. This will give us flexibility to change
** the namespace for different releases in the future.
**
** See also spi/makefiles/version.mk
**
** The version numbers defined in this file must match those defined in
** version.mk
***************************************************************************
*/

#ifndef SPI_NAMESPACE_HPP
#define SPI_NAMESPACE_HPP

#undef SPI_BEGIN_NAMESPACE
#undef SPI_END_NAMESPACE

#undef SPI_VERSION
#define SPI_VERSION 0.1.6.0

#if defined SPI_STATIC || !defined SPI_VERSION

// for the static library build we don't bother with the versioned namespace
// and alias - the static library is not part of the general release

#define SPI_NAMESPACE spi
#define SPI_BEGIN_NAMESPACE namespace SPI_NAMESPACE {
#define SPI_END_NAMESPACE }

#else

#define SPI_NAMESPACE spi_016
#define SPI_BEGIN_NAMESPACE namespace SPI_NAMESPACE {
#define SPI_END_NAMESPACE }

SPI_BEGIN_NAMESPACE
SPI_END_NAMESPACE

// defines an alias so that we can use spi:: in calling code
// note that we have to use the namespace before we can define an alias to it
namespace spi = SPI_NAMESPACE;

#endif /* SPI_STATIC */

#endif /* SPI_NAMESPACE_HPP */

