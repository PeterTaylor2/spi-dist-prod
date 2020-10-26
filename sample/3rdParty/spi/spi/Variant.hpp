#ifndef SPI_VARIANT_HPP
#define SPI_VARIANT_HPP

/*
***************************************************************************
** Variant.hpp
***************************************************************************
** Polymorphic class with value semantics, arbitrary type and late binding.
**
** Value semantics means that Variant can be assigned by value and will
** correctly handle all memory issues.
**
** Arbitrary type means any of the built-in types.
**
** Late binding means that sometimes you only discover the type in the
** context of the interface. For example, when using Excel you don't
** know whether a number should be treated as an int, a double or a date.
** Variant class won't know, but in context the correct type will be
** requested.
***************************************************************************
*/

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

#include "Value.hpp"
#include "MatrixData.hpp"

SPI_BEGIN_NAMESPACE

class InputContext;

class SPI_IMPORT Variant
{
public:

    // this constructor of Variant is used by the interface layer
    // to create a Variant in the context of the calling application
    Variant (const Value& value,
             const InputContext* context);

    // this constructor of Variant is used when translating outputs
    // essentially there is no input context
    Variant (const Value& value);

    /* Value semantics needed */
    ~Variant ();
    Variant (const Variant &value);
    Variant& operator= (const Variant &value);

    /**
     * Translation between Variant and Map for serialization.
     */
    Variant(const MapConstSP& m);
    MapConstSP ToMap() const;

    /**
     * Data accessors.
     *
     * In general these functions are as forgiving as possible if the
     * type is not an exact match.
     */
    Value::Type      ValueType() const;
    bool             IsUndefined() const;
    char             ToChar() const;
    std::string      ToString() const;
    int              ToInt() const;
    bool             ToBool() const;
    double           ToDouble() const;
    Date             ToDate() const;
    ObjectConstSP    ToConstObject() const;

    // for scalar access we can also use cast operators instead
    // of the To... methods
    operator char() const;
    operator std::string() const;
    operator int() const;
    operator bool() const;
    operator double() const;
    operator Date() const;
    operator ObjectConstSP() const;

    std::vector<std::string>      ToStringVector() const;
    std::vector<double>           ToDoubleVector() const;
    std::vector<int>              ToIntVector() const;
    std::vector<bool>             ToBoolVector() const;
    std::vector<Date>             ToDateVector() const;
    std::vector<ObjectSP>         ToObjectVector() const;
    std::vector<ObjectConstSP>    ToConstObjectVector() const;

    /**
     * Converts a Variant of type array to a scalar.
     *
     * If the Variant is not an array, then this returns a copy of the
     * variant.
     *
     * If the Variant is an array of size 0, then this returns Undefined.
     * If the Variant is an array of size 1, then this returns that value.
     * If the Variant is an array of size > 1 then this will throw an
     * exception.
     */
    Variant ToScalar() const;

    // these constructors of Variant are designed for returning
    // outputs from functions when the output is declared as Variant
    Variant ();
    Variant (char value);
    Variant (const char *value);
    Variant (const std::string &value);
    Variant (int value);
    Variant (double value);
    Variant (bool value);
    Variant (const Date &value);
    Variant (const ObjectConstSP &value);

    template<typename T>
    Variant (const std::vector<T> &value);

    // this is used internally to convert Variant to Value on output
    // this function fails if the input context is defined
    operator Value() const;

    const Value& GetValue() const;
    const char* GetContext() const;
    const InputContext* GetInputContext() const;

private:

    Value               m_value;
    const InputContext* m_context;
};

template<typename T>
Variant::Variant(const std::vector<T> &vec)
:
m_value(vec),
m_context(0)
{}

#if 0
// specialisation of MatrixData<Variant> templates
// these are needed since MatrixData<T> assumes that T can be converted to/from Value
// whereas for Variant we need to go via Map to do this conversion
template<>
inline MapConstSP MatrixData<Variant>::ToMap() const
{
    MapSP m(new Map(Map::MATRIX));

    m->SetValue("rows", (int)this->Rows());
    m->SetValue("cols", (int)this->Cols());

    const std::vector<Variant>& data = this->Data();
    size_t size = data.size();
    std::vector<MapConstSP> mdata;
    for (size_t i = 0; i < size; ++i)
        mdata.push_back(data[i].ToMap());

    m->SetValue("data", mdata);

    return m;
}
#endif

// note that we no longer encode a matrix via a map, but for backward
// compatibility we need to support this format
template<>
inline MatrixData<Variant> MatrixData<Variant>::FromMap(const MapConstSP& m)
{
    size_t nbRows = m->GetValue("rows").getInt();
    size_t nbCols = m->GetValue("cols").getInt();
    const std::vector<MapConstSP>& mdata = m->GetValue("data").getVector<MapConstSP>();

    std::vector<Variant> data;
    for (size_t i = 0; i < mdata.size(); ++i)
    {
        data.push_back(Variant(mdata[i]));
    }

    return MatrixData<Variant>(nbRows, nbCols, data);
}

// specialisation of MatrixData<Variant> templates
// these are needed since MatrixData<T> assumes that T can be converted to/from Value
// whereas for Variant we need to go via Map to do this conversion
template<>
inline IArrayConstSP MatrixData<Variant>::ToArray() const
{
    std::vector<size_t> dims;
    dims.push_back(Rows());
    dims.push_back(Cols());

    const std::vector<Variant>& data = Data();
    size_t size = data.size();
    std::vector<Value> values;
    for (size_t i = 0; i < size; ++i)
        values.push_back(data[i].ToMap());

    return IArrayConstSP(new ValueArray(values, dims));
}

template<>
inline MatrixData<Variant> MatrixData<Variant>::FromValueMatrix(const MatrixData<Value>& vmat)
{
    const std::vector<Value>& values = vmat.Data();

    std::vector<Variant> data;
    for (size_t i = 0; i < values.size(); ++i)
    {
        data.push_back(Variant(values[i].getMap()));
    }

    return MatrixData<Variant>(vmat.Rows(), vmat.Cols(), data);
}

SPI_END_NAMESPACE

#endif

