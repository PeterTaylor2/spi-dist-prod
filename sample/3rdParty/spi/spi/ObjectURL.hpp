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
#ifndef SPI_OBJECT_URL_HPP
#define SPI_OBJECT_URL_HPP

/*
 * Isolates all URL based functions to improve static linking.
 */

#include "Object.hpp"

#include <fstream>
#include <vector>
#include <list>

SPI_BEGIN_NAMESPACE

SPI_DECLARE_RC_CLASS(Service);
class InputContext;

std::string read_url(const std::string& url,
    int timeout,
    int cacheAge);

void url_cache_clear_entry(const std::string& url);
void url_cache_clear();
void url_cache_init();
void url_cache_load(const char* filename);
void url_cache_save(const char* filename);
size_t url_cache_size();

ObjectConstSP object_from_url(
    const ServiceConstSP& service,
    const std::string& url,
    int timeout = 0,
    int cacheAge = 0);

SPI_END_NAMESPACE

#endif
