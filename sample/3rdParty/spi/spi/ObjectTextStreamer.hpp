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
** ObjectTextStreamer.hpp
***************************************************************************
** Defines streaming interface for objects.
***************************************************************************
*/

#ifndef SPI_OBJECT_TEXT_STREAMER_HPP
#define SPI_OBJECT_TEXT_STREAMER_HPP

#include "IObjectStreamer.hpp"

SPI_BEGIN_NAMESPACE

class Value;
SPI_DECLARE_RC_CLASS(Map);
SPI_DECLARE_RC_CLASS(ObjectTextStreamer);
SPI_DECLARE_RC_CLASS(ObjectRefCache);
SPI_DECLARE_RC_CLASS(ObjectCompressedTextStreamer);

// the point of this class is to maintain a reference from id (part of
// each object - count starts at 1 each time we start the process) and
// mapRef (count starts at 1 each time we start a map or start to stream
// objects to file)
//
// we also find that we need to keep track of objects created - this is
// because sometimes when converting an object to map we find that we
// create new wrapper class instances and that we need to keep a handle on
// these in order to keep track of identical objects
class SPI_IMPORT MapRefCache
{
public:
    MapRefCache();
    size_t count(int id) const;
    size_t size() const;

    void insert(int mapRef, const ObjectConstSP& obj);
    int mapRef(int id) const;

private:
    std::map<int,int> m_indexIdMapRef;
    std::vector<ObjectConstSP> m_objects;

    MapRefCache(const MapRefCache&);
    MapRefCache& operator=(const MapRefCache&);
};


/**
 * ObjectTextStreamer interface.
 *
 * Implements to_ostream and from_istream for an object using a text format.
 */
class SPI_IMPORT ObjectTextStreamer : public IObjectStreamer
{
public:
    // functions to implement IObjectTextStreamer
    ObjectConstSP from_stream(
        const std::string& streamName,
        std::istream& istr,
        const MapConstSP& metaData);
    void to_stream(
        std::ostream& ostr,
        const ObjectConstSP& object,
        const MapConstSP& metaData = MapConstSP());

    // functions specific to ObjectTextStreamer

    // converts text to object - called after extracting the text from a
    // stream (which might have been a compressed stream)
    ObjectConstSP from_text(
        const std::string& streamName,
        const std::string& contents,
        const MapConstSP& metaData
    );

    // writes a field to an output stream in the form of a single value Map
    // without a class name or reference number
    void to_stream(std::ostream& ostr, const char* name, const Value& value);
    void to_stream(std::ostream& ostr, const MapConstSP& m);

    /**
     * If options is defined then it is a ';' delimited string with the
     * following options supported (case independent):
     *
     * ACC: indicates maximum accuracy of doubles is provided in the floating
     *      point string representation (i.e. 17 decimal places) as opposed
     *      to 15 decimal places which ensures removal of trailing 0s and 9s
     *      for numbers entered with a fixed number of decimal places.
     * NOREF: Don't show object reference numbers on output.
     * LOACC: indicates minimum accuracy (i.e. 8 decimal places).
     */
    static ObjectTextStreamerSP Make(
        const ServiceConstSP& service,
        const char* options=0);

    static void Register();

    ~ObjectTextStreamer();

private:
    // main constructor - accessed via static Make method
    ObjectTextStreamer(
        const ServiceConstSP& service,
        const char* options);

    // prevent compiler implementation of copy constructor and assignment
    ObjectTextStreamer(const ObjectTextStreamer&);
    ObjectTextStreamer& operator=(const ObjectTextStreamer&);

    ServiceConstSP   m_service;
    ObjectRefCacheSP m_objectCache; // used when reading from file
    MapRefCache      m_mapRefCache; // used when writing to file
    bool             m_maxAccuracy;
    bool             m_minAccuracy;
    bool             m_noMapRef;
    bool             m_noIndent;
};

/**
 * ObjectCompressedTextStreamer interface.
 *
 * Implements to_ostream and from_istream for an object using a text format.
 */
class SPI_IMPORT ObjectCompressedTextStreamer : public IObjectStreamer
{
public:
    // implementation of IObjectTextStreamer
    ObjectConstSP from_stream(
        const std::string& streamName,
        std::istream& istr,
        const MapConstSP& metaData);
    void to_stream(
        std::ostream& ostr,
        const ObjectConstSP& object,
        const MapConstSP& metaData);

    /**
     * If options is defined then it is a ';' delimited string with the
     * same options supported as ObjectTextStreamer.
     */
    static ObjectCompressedTextStreamerSP Make(
        const ServiceConstSP& service,
        const char* options=0);

    static void Register();

    ~ObjectCompressedTextStreamer();

private:
    // main constructor - accessed via static Make method
    ObjectCompressedTextStreamer(
        const ServiceConstSP& service,
        const char* options);

    // prevent compiler implementation of copy constructor and assignment
    ObjectCompressedTextStreamer(const ObjectCompressedTextStreamer&);
    ObjectCompressedTextStreamer& operator=(const ObjectCompressedTextStreamer&);

    ObjectTextStreamerSP m_textStreamer;
};

SPI_END_NAMESPACE

#endif

