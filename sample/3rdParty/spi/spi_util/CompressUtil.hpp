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
** CompressUtil.hpp
***************************************************************************
** Compression functions implemented using the industry standard zlib.
***************************************************************************
*/

#ifndef SPI_UTIL_COMPRESS_UTIL_HPP
#define SPI_UTIL_COMPRESS_UTIL_HPP

#include "DeclSpec.h"
#include "Namespace.hpp"

#include <string>

SPI_UTIL_NAMESPACE

/*
 * Defines a data buffer for use with the compression routines.
 *
 * You can construct from a pointer and size (in which case it is
 * your pointer and it won't be freed when the DataBuffer goes out
 * of scope).
 *
 * After construction, you can subsequently allocate data - in which
 * case the data is owned by the DataBuffer and will be freed when the
 * DataBuffer goes out of scope.
 */
struct DataBuffer
{
    DataBuffer(const std::string& data);
    DataBuffer();

    void Allocate(size_t newSize);

    void* data; // &str[0]
    size_t size;
    std::string str; // actual data storage

private:
    DataBuffer(const DataBuffer&);
    DataBuffer& operator=(const DataBuffer&);
};

/**
 * Uses ZLIB to compress the input buffer and return allocated output.
 *
 * Note that the output will contain the length of the original input at
 * the start (a requirement for UncompressData function below).
 */
void CompressData(DataBuffer& out, const DataBuffer& input, int level);

/**
 * Uses ZLIB to uncompress the input buffer and return allocated output.
 *
 * Assumes that the compressed buffer contains the length of the original
 * input at the start in order to correctly allocate the size of the output.
 */
void UncompressData(DataBuffer& out, const DataBuffer& input);

/**
 * Compresses the input and then performs base64 encoding to convert
 * the result to a string.
 */
std::string CompressDataAndEncode(const DataBuffer& in, int level);

/**
 * Performs base64 decoding on the input string, and then uncompresses
 * the resulting data to create the output.
 */
void DecodeTextAndUncompress(DataBuffer& out, const std::string& input);

/**
 * Compresses a text string using ZLIB and then base64 encodes the result.
 */
SPI_UTIL_IMPORT
std::string CompressText(const std::string& str);

/**
 * Performs base64 decoding on the string and then uncompresses the
 * resulting data.
 *
 * The string provided is modified to reduce copying of strings.
 *
 * If the output is not a null-terminated string then an exception is thrown.
 */
SPI_UTIL_IMPORT
void UncompressText(std::string& str);

SPI_UTIL_END_NAMESPACE

#endif


