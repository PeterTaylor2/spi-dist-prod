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
#ifndef SPI_UTIL_URL_READ_HPP
#define SPI_UTIL_URL_READ_HPP

/*
***************************************************************************
** UrlRead.hpp
***************************************************************************
** Functions for reading the entire contents for a URL.
**
** Wrapper to curl.lib (or libcurl.a)
**
** This module will be the only module which uses curl.lib so if you don't
** use anything in this module then you won't need to link with curl.lib.
***************************************************************************
*/

#include "DeclSpec.h"
#include "Namespace.hpp"
#include <string>

SPI_UTIL_NAMESPACE

/**
 * Reads the entire contents for the given URL.
 *
 * No caching is performed by this function.
 *
 * @param url
 * @param noProxy
 *    If this is defined then we will not attempt to use the proxy server
 *    for reading the URL.
 * @param timeout
 *    If this is positive then it is a timeout in seconds.
 *    If this is zero then the function always fails by returning an empty string.
 *    If this is negative then there is no timeout.
 */
SPI_UTIL_IMPORT
std::string URLReadContents(
    const std::string& url,
    bool noProxy,
    int timeout);

SPI_UTIL_END_NAMESPACE

#endif

