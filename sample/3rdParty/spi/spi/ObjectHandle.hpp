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
#ifndef SPI_OBJECT_HANDLE_HPP
#define SPI_OBJECT_HANDLE_HPP

/*
***************************************************************************
** ObjectHandle.hpp
**
** Defines functions for mapping Objects to string and back again.
**
** Although this is only going to be used by Excel, we keep the code at
** a higher level so that other platforms can read/write object handles
** if they are interacting with Excel at some other level.
***************************************************************************
*/

#include "DeclSpec.h"
#include "Namespace.hpp"
#include "Object.hpp"
#include "Map.hpp"

SPI_BEGIN_NAMESPACE

/**
 * Stores an object and returns the corresponding handle to get it back again.
 */
SPI_IMPORT
std::string ObjectHandleSave(
    const std::string&   baseName,
    const ObjectConstSP& object,
    bool                 noCount=false);

/**
 * Finds an object given its handle.
 */
SPI_IMPORT
ObjectConstSP ObjectHandleFind(
    const std::string& handle);

SPI_IMPORT
int ObjectHandleFreeAll();

SPI_IMPORT
int ObjectHandleCount(const std::string& className);

SPI_IMPORT
void ObjectHandleFree(const std::string& handle);

SPI_IMPORT
std::vector<std::string> ObjectHandleList(
    const std::string& baseName,
    const std::string& className);

SPI_IMPORT
std::string ObjectHandleClassName(
    const std::string& handle);

bool IsNullHandle(const std::string& handle);

SPI_END_NAMESPACE

#endif
