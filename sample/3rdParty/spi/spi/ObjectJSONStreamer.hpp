/*
***************************************************************************
** ObjectJSONStreamer.hpp
***************************************************************************
** Defines streaming interface for objects using JSON.
**
** Note that we don't try to catch duplicate objects using JSON.
** Hence we won't be including the object references.
***************************************************************************
*/

#ifndef SPI_OBJECT_JSON_STREAMER_HPP
#define SPI_OBJECT_JSON_STREAMER_HPP

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

#include "IObjectStreamer.hpp"

SPI_BEGIN_NAMESPACE

class Value;
SPI_DECLARE_RC_CLASS(ObjectJSONStreamer);

/**
 * ObjectJSONStreamer interface.
 *
 * Implements to_ostream and from_istream for an object using a text format.
 */
class SPI_IMPORT ObjectJSONStreamer : public IObjectStreamer
{
public:
    // implementation of IObjectStreamer
    ObjectConstSP from_stream(
        const std::string& streamName,
        std::istream& istr,
        const MapConstSP& metaData);
    void to_stream(
        std::ostream& ostr,
        const ObjectConstSP& object,
        const MapConstSP& metaData);

    // writes a field to an output stream in the form of a single value Map
    // without a class name or reference number
    void to_stream(std::ostream& ostr, const char* name, const Value& value);

    static ObjectJSONStreamerSP Make(
        const ServiceConstSP& service,
        const char* options=0);

    static void Register();

    // re-implements IObjectStreamer
    bool uses_recognizer() const;

    ~ObjectJSONStreamer();

private:
    // main constructor - accessed via static Make method
    ObjectJSONStreamer(
        const ServiceConstSP& service,
        const char* options);

    // prevent compiler implementation of copy constructor and assignment
    ObjectJSONStreamer(const ObjectJSONStreamer&);
    ObjectJSONStreamer& operator=(const ObjectJSONStreamer&);

    ServiceConstSP m_service;
    bool m_noNewLine;
};

SPI_END_NAMESPACE

#endif

