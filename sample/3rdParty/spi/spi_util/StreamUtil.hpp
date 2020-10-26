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
/**
***************************************************************************
** StreamUtil.hpp
***************************************************************************
** Stream utility functions.
***************************************************************************
*/

#ifndef SPI_UTIL_STREAM_UTIL_HPP
#define SPI_UTIL_STREAM_UTIL_HPP

#include "DeclSpec.h"
#include "Namespace.hpp"

#include <vector>
#include <string>
#include <iostream>

SPI_UTIL_NAMESPACE

SPI_UTIL_IMPORT
std::vector<std::string> StreamReadLines(std::istream& str);

SPI_UTIL_END_NAMESPACE

#endif

