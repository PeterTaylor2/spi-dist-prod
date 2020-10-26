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
#ifndef SPI_INPUT_VALUES_HPP
#define SPI_INPUT_VALUES_HPP

/**
 * The goal is to enable an arbitrary platform to have all of its functions
 * implemented as follows (using Excel by example):
 *

xloper* xlFunc(xloper* a1, xloper* a2, xloper* a3)
{
    static FunctionCaller* func = 0;
    if (!func)
        func = svc->get_function_caller("Func");

    InputValues iv = ExcelInputValues(func, a1, a2, a3);
    Value ov = CallInContext(func, iv, ExcelContext());
    return ExcelOutputValue(ov);
}

 *
 * In this example, ExcelInputValues is responsible for converting from
 * xloper* to Value. This may result in an InputValues object which has
 * permuted inputs.
 *
 * Then CallInContext will be responsible for looping through the
 * permutations, and for each permutation will handle the late-binding of
 * the Value to some given type. Thus the FuncDef must have a method for
 * calling the function from a vector of Value of the correct type.
 *
 * Finally CallInContext will return an output as Value which then needs
 * to be converted to an xloper using ExcelOutputValue.
 *
 * Where are all these objects?
 *
 * FuncDef is defined within the DLL. There will be a look-up by name.
 * ExcelInputValues will be a generic function within spxl.lib.
 * CallInContext will be a generic function within spi.dll.
 * ExcelOutputValue will be a generic function within spxl.lib.
 *
 * Note that there are three types of function that we need to support.
 *
 * 1. Regular function.
 * 2. Class constructor.
 * 3. Class method.
 *
 * Class constructors will not support permuted arguments.
 * Regular functions will support permuted arguments.
 * Class methods will not allow the class instance to be permuted.
 */

#include "Value.hpp"
#include "Object.hpp"

#include "InputContext.hpp"
#include "FunctionCaller.hpp"

SPI_BEGIN_NAMESPACE

SPI_DECLARE_RC_CLASS(Service);
class ObjectType;

class SPI_IMPORT InputValues
{
public:
    InputValues(const char* name);

    void AddValue(
        const FuncArg& arg,
        const Value&   value);

    std::vector<Value> GetPermutation(
        const InputContext*   context,
        const FunctionCaller* func,
        size_t permutation) const;
    bool isPermuted() const;
    size_t nbPermutations() const;

private:
    const char*        m_name;
    std::vector<Value> m_inputs;
    bool               m_isPermuted;
    size_t             m_nbPermutations;
};

SPI_IMPORT
Value CallInContext(
    const FunctionCaller* func,
    const InputValues&  inputValues,
    const InputContext* inputContext);

SPI_END_NAMESPACE

#endif

