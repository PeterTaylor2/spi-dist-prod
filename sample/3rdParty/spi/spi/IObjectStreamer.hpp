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
** IObjectStreamer.hpp
***************************************************************************
** Defines streaming interface for objects.
***************************************************************************
*/

#ifndef SPI_IOBJECT_STREAMER_HPP
#define SPI_IOBJECT_STREAMER_HPP

#include "RefCounter.hpp"
#include "Map.hpp"

#include <map>
#include <istream>
#include <ostream>
#include <string>
#include <vector>

SPI_BEGIN_NAMESPACE

SPI_DECLARE_RC_CLASS(IObjectStreamer);
SPI_DECLARE_RC_CLASS(Object);
SPI_DECLARE_RC_CLASS(Service);
SPI_DECLARE_RC_CLASS(Map);

/**
 * IObjectStreamer interface.
 *
 * An object streamer describes the way that objects are written to and from
 * C++ stream-like classes.
 */
class SPI_IMPORT IObjectStreamer : public RefCounter
{
public:
    /**
     * Defines the constructors of the various types of IObjectStream
     * available in the library.
     *
     * Initially we will support an easy to read text format.
     *
     * Subsequently we expect to add binary formats with the field names
     * excluded.
     */
    typedef IObjectStreamerSP (Maker) (const ServiceConstSP& service,
                                       const char* options);

    virtual ObjectConstSP from_stream(
        const std::string& streamName, // for error messages only
        std::istream& istr,
        const MapConstSP& metaData = MapConstSP()) = 0;
    virtual void to_stream(
        std::ostream& ostr,
        const ObjectConstSP& object,
        const MapConstSP& metaData = MapConstSP()) = 0;

    // by default the streamer does not use the recognizer string
    virtual bool uses_recognizer() const;
    virtual bool is_binary() const;

    static IObjectStreamerSP Make(
        const ServiceConstSP& service,
        const std::string& key,
        const char* options=0);

    static const char* Recognizer(const std::string& key);

    static IObjectStreamerSP MakeWriter(
        const std::string& key,
        const char* options=0);

    static void Register(const std::string& key, Maker* maker,
        bool isBinary, const char* recognizer);
    static std::vector<std::string> Formats(bool allowBinary);

private:
    struct RegInfo
    {
        RegInfo(Maker*, bool, const char*);
        RegInfo();

        Maker*      maker;
        bool        isBinary;
        const char* recognizer;
    };

    static std::map<std::string, RegInfo> g_registry;
    static std::vector<std::string> g_formats;

    static RegInfo FindRegInfo(const std::string& key);
};

SPI_END_NAMESPACE

#endif

