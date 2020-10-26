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
#ifndef SPI_OPTIONS_PARSER_HPP
#define SPI_OPTIONS_PARSER_HPP

#include "Value.hpp"
#include "Map.hpp"

SPI_BEGIN_NAMESPACE

class SPI_IMPORT OptionsParser
{
public:
    OptionsParser(const std::string& name);

    void Parse(const char* str);

    // call these functions before calling Parse to set the defaults
    void SetChar(const std::string& name, char value);
    void SetInt(const std::string& name, int value);
    void SetDouble(const std::string& name, double value);
    void SetDate(const std::string& name, Date value);
    void SetString(const std::string& name, const std::string& value);
    void SetBool(const std::string& name, bool value);

    // call these functions after calling Parse to extract the values to use
    char GetChar(const std::string& name) const;
    int GetInt(const std::string& name) const;
    double GetDouble(const std::string& name) const;
    Date GetDate(const std::string& name) const;
    std::string GetString(const std::string& name) const;
    bool GetBool(const std::string& name) const;

private:
    std::string                  m_name;
    std::map<std::string, Value> m_options;
    std::map<std::string, Value> m_defaults;

    OptionsParser(const OptionsParser&);
    OptionsParser& operator=(const OptionsParser&);
};

SPI_END_NAMESPACE

#endif
