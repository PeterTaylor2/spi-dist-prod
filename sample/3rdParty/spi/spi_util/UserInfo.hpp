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
#ifndef SPI_USER_INFO_HPP
#define SPI_USER_INFO_HPP

/*
***************************************************************************
* UserInfo.hpp
***************************************************************************
* Information about the currently logged on user.
***************************************************************************
*/

#include "DeclSpec.h"
#include "Namespace.hpp"

#include <string>
#include <vector>

SPI_UTIL_NAMESPACE

// returns the name of the currently logged on user
SPI_UTIL_IMPORT
std::string UserName();

// returns the name of the current computer
SPI_UTIL_IMPORT
std::string ComputerName();

// returns the names of the groups to which the a given user belongs
// by default the current user is used if no userName provided
SPI_UTIL_IMPORT
std::vector<std::string> UserGroups
(const char* serverName=NULL,
 const char* userName=NULL);

SPI_UTIL_END_NAMESPACE

#endif
