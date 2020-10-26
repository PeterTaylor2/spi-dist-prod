#ifndef SPI_UTIL_JSON_PARSER_HPP
#define SPI_UTIL_JSON_PARSER_HPP

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
** JSONParser.hpp
***************************************************************************
** Parses a stream and returns a JSON-style map.
***************************************************************************
*/

#include "DeclSpec.h"
#include "Namespace.hpp"
#include "RefCounter.hpp"

#include <string>
#include <map>
#include <vector>
#include <iostream>

SPI_UTIL_NAMESPACE

class JSONValue;

SPI_UTIL_IMPORT
void JSONValueToStream(
    std::ostream& str,
    const JSONValue& value,
    bool noNewLine = false,
    size_t indent = 0);

SPI_UTIL_IMPORT
JSONValue JSONParseValue(
    std::istream& istr,
    const std::string& streamName=std::string());

//SPI_UTIL_IMPORT
JSONValue JSONValueFromString(
    std::string& str);

//SPI_UTIL_IMPORT
std::string JSONValueToString(const JSONValue& value);

SPI_UTIL_END_NAMESPACE

#endif
