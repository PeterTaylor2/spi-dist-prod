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
/*
***************************************************************************
** Defines the Object reference class and various useful macros
***************************************************************************
*/
#ifndef SPI_CLI_OBJECT_HPP
#define SPI_CLI_OBJECT_HPP

#using <mscorlib.dll>
using namespace System::Runtime::InteropServices;

#include <spi/spi.hpp>
#include <spi/ObjectPut.hpp>
#include "cliInput.hpp"
#include "cliOutput.hpp"

#define DECLARE_ATTRIBUTE(TYPE,NAME)\
property TYPE NAME\
{\
    TYPE get();\
    void set(TYPE value);\
}

#define DECLARE_PROPERTY(TYPE,NAME)\
property TYPE NAME\
{\
    TYPE get();\
}

namespace SPI {

ref class Variant;
ref class Map;

public ref class Object : public System::Object
{
public:
    ~Object();

    static Object^ Wrap(const ::spi::Object* self);

    System::String^ to_string(
        [OptionalAttribute] System::String^ format, 
        [OptionalAttribute] System::String^ options, 
        [OptionalAttribute] Map^ metaData);
    System::String^ to_file(
        System::String^ filename, 
        [OptionalAttribute] System::String^ format,
        [OptionalAttribute] System::String^ options, 
        [OptionalAttribute] Map^ metaData);
    Variant^ get_value(System::String^ str);

internal:
    Object();
    void set_inner(const ::spi::Object* self);
    const ::spi::Object* self;
    virtual void make_inner_delegate();
    virtual Map^ to_map();
};

// NOTE: Seems like an excessive number of pointers.
//       However not allow public value class Variant with mixed types.
//       We are not allowed ::spi::Variant value inside Variant with mixed types.
public ref class Variant : public System::Object
{
public:
    Variant(System::String^ str);
    Variant(System::DateTime dt);
    Variant(System::Double d);
    Variant(System::Boolean b);
    Variant(System::Int32 i);

    ~Variant();

    static operator System::DateTime(Variant^);
    static operator System::String^(Variant^);
    static operator System::Double(Variant^);
    static operator System::Int32(Variant^);
    static operator System::Boolean(Variant^);
    static operator cli::array<System::DateTime>^ (Variant^);
    static operator cli::array<System::String^>^ (Variant^);
    static operator cli::array<System::Double>^ (Variant^);
    static operator cli::array<System::Int32>^ (Variant^);
    static operator cli::array<System::Boolean>^ (Variant^);

internal:
    Variant(const ::spi::Variant& var);
    static cli::array<Variant^>^ MakeArray(const std::vector<::spi::Variant>& in);
    static std::vector<::spi::Variant> FromArray(cli::array<Variant^>^ in);
    static cli::array<Variant^,2>^ MakeArray2(const ::spi::MatrixData<::spi::Variant>& in);
    static ::spi::MatrixData<::spi::Variant> FromArray2(cli::array<Variant^,2>^ in);

    ::spi::Variant* self;
};

public ref class Map : public Object
{
public:
    void set_value(System::String^ name, Variant^ value);
    static Map^ Make(System::String^ className);
private:
    Map();
internal:
    ~Map();
    void set_inner(::spi::MapObject* self);
    static Map^ Wrap(::spi::MapObject* self);

    ::spi::MapObject* self;
    static ::spi::MapConstSP get_map(Map^ aMap);
};

} // end namespace SPI


#endif /* SPI_CLI_OBJECT_HPP */

