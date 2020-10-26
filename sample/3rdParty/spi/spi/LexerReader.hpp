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
#ifndef SPI_LEXER_READER_HPP
#define SPI_LEXER_READER_HPP

/*
***************************************************************************
** LexerReader.hpp
***************************************************************************
** Functions for reading various standard types from a Lexer.
***************************************************************************
*/

#include "Value.hpp"
#include "Array.hpp"

namespace spi_util
{
    class Lexer;
}

SPI_BEGIN_NAMESPACE

SPI_DECLARE_RC_CLASS(Map);
SPI_DECLARE_RC_CLASS(ValueArray);
SPI_DECLARE_RC_CLASS(ObjectReader);

Value ReadValue(spi_util::Lexer* lexer);
MapSP ReadMap(spi_util::Lexer* lexer);
ValueArraySP ReadArray(spi_util::Lexer* lexer);

SPI_END_NAMESPACE

#endif

