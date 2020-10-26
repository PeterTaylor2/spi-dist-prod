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
#ifndef SPI_FUNCTION_CALLER_HPP
#define SPI_FUNCTION_CALLER_HPP

#include "Value.hpp"

SPI_BEGIN_NAMESPACE

namespace ArgType
{
    enum Enum
    {
        BOOL,
        CHAR,
        INT,
        DOUBLE,
        STRING,
        DATE,
        DATETIME,
        ENUM,
        OBJECT,
        VARIANT
    };
}

struct FuncArg
{
    const char*       name;
    ArgType::Enum     argType;
    const char*       typeName;
    bool              isArray;
    bool              isOptional;
    bool              isPermutable;
};

struct FunctionCaller
{
    const char*    name;
    size_t         nbArgs;
    const FuncArg  args[50];
    Value (*caller)(const InputContext* context,
        const std::vector<Value>& value);
};

SPI_END_NAMESPACE

#endif

