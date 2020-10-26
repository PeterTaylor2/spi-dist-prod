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
#ifndef SPI_FUNCTION_HPP
#define SPI_FUNCTION_HPP

/*
***************************************************************************
** Function.hpp
***************************************************************************
** Defines the Function class which is a sub-class of Object.
**
** The function will contain a function caller and a map of inputs.
** This will be enough for serialization and for calling the function.
** The question of registration is something else entirely.
***************************************************************************
*/

#include "Object.hpp"
#include "Value.hpp"
#include "Map.hpp"
#include "InputValues.hpp"

SPI_BEGIN_NAMESPACE

class Service;

SPI_DECLARE_RC_CLASS(Function);

class SPI_IMPORT Function : public Object
{
public:
    Function(
        const Service* service,
        const char* name,
        const std::vector<Value>& inputs);
    Function(
        const ServiceConstSP& service,
        FunctionCaller* caller, 
        const std::vector<Value>& inputs);
    static ObjectConstSP MakeFromMap(
        IObjectMap* m,
        ValueToObject& valueToObject);
    static bool IsInstance(const ObjectConstSP& o);

    Value call() const;

    // implementation of Object interface
    // get_object_type could be a problem
    void to_map(IObjectMap* objectMap, bool publicOnly) const;
    ObjectType* get_object_type() const;

private:
    ServiceConstSP m_service;
    FunctionCaller* m_caller;
    std::vector<Value> m_inputs;
    ObjectType* m_objectType;
};

SPI_IMPORT
ObjectType FunctionObjectType(const char* name);

SPI_END_NAMESPACE

#endif
