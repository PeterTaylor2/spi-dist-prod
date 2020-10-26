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
#ifndef SPI_FOUR_PART_VERSION_HPP
#define SPI_FOUR_PART_VERSION_HPP

/*
***************************************************************************
* FourPartVersion.hpp
***************************************************************************
* Class for dealing with 4-part version numbers.
* Numbers allowed are in the range 0..99
***************************************************************************
*/

#include "DeclSpec.h"
#include "Namespace.hpp"

#include <string>

SPI_UTIL_NAMESPACE

class SPI_UTIL_IMPORT FourPartVersion
{
public:
    FourPartVersion(int v1, int v2, int v3, int v4);
    FourPartVersion(unsigned int version);
    FourPartVersion(const char* version);
    FourPartVersion(const std::string& version);

    unsigned int version() const;
    int v1() const;
    int v2() const;
    int v3() const;
    int v4() const;
    std::string versionString() const;

private:
    int m_v1;
    int m_v2;
    int m_v3;
    int m_v4;

    void init(int v1, int v2, int v3, int v4);
    void init(const char* version);
};

SPI_UTIL_END_NAMESPACE

#endif
