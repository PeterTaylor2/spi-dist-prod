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
#ifndef SPI_MAP_READER_HPP
#define SPI_MAP_READER_HPP

/*
***************************************************************************
** MapReader.hpp
***************************************************************************
** Functions for reading various standard types from an input stream.
***************************************************************************
*/

#include "Value.hpp"
#include "Array.hpp"

SPI_BEGIN_NAMESPACE

class MapReader
{
public:

    enum TokenType
    {
        STRING,
        CHAR,
        SEPARATOR,
        SYMBOL,
        NONE
    };

    MapReader (const std::string& filename,
        const char* contents);

    // returns false if there are no more tokens
    bool read(std::string& token, TokenType& type);
    int lineno() const;
    const std::string& filename() const;

    void returnToken(const std::string& token, TokenType type);

private:

    MapReader(const MapReader&);
    MapReader& operator=(const MapReader&);

    std::string m_filename;
    const char* m_contents; // original
    const char* m_ptr; // current pointer
    int m_lineno;
    std::string m_line; // remainder of the current line
    std::string m_returnedToken;
    TokenType m_returnedType;

    void skipWhiteSpace();
    void skipToEOL();
};

SPI_DECLARE_RC_CLASS(Map);
SPI_DECLARE_RC_CLASS(ValueArray);
SPI_DECLARE_RC_CLASS(ObjectReader);

MapSP ReadMap(MapReader* reader, int format=2);
Value ReadValue(MapReader* reader, int format);
ValueArraySP ReadArray(MapReader* reader, int format);

SPI_END_NAMESPACE

#endif

