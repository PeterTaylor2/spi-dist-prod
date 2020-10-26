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
** cliInput.hpp
**
** Functions with convert from the native C++/CLI data types to various
** standard and spi-specific c++ data types.
**
** Each function has a scalar and a vector equivalent.
***************************************************************************
*/

#ifndef SPI_CLI_INPUT_HPP
#define SPI_CLI_INPUT_HPP

#using <mscorlib.dll>

#include <spi/Date.hpp>
#include <spi/DateTime.hpp>
#include <spi/MatrixData.hpp>

#include <string>
#include <vector>

SPI_BEGIN_NAMESPACE

// boolean input conversion
bool ToBool(System::Boolean in);
std::vector<bool> ToBoolVector(cli::array<System::Boolean>^ in);
spi::MatrixData<bool> ToBoolMatrix(cli::array<System::Boolean,2>^ in);

// integer input conversion
int ToInt(System::Int32 in);
std::vector<int> ToIntVector(cli::array<System::Int32>^ in);
spi::MatrixData<int> ToIntMatrix(cli::array<System::Int32,2>^ in);

//long ToLong(System::Int32 in);
//std::vector<long> ToLongVector(cli::array<System::Int32>^ in);

// unsigned integer input conversion
//int ToSizeT(System::UInt32 in);
//std::vector<size_t> ToSizeTVector(cli::array<System::UInt32>^ in);

// floating point input conversion
double ToDouble(System::Double in);
std::vector<double> ToDoubleVector(cli::array<System::Double>^ in);
spi::MatrixData<double> ToDoubleMatrix(cli::array<System::Double,2>^ in);

// string input conversion
std::string ToString(System::String^ in);
std::vector<std::string> ToStringVector(cli::array<System::String^>^ in);
spi::MatrixData<std::string> ToStringMatrix(cli::array<System::String^,2>^ in);

// character input conversion
char ToChar(System::SByte in);
std::vector<char> ToCharVector(cli::array<System::SByte>^ in);

// date input conversion
Date ToDate(System::DateTime in);
std::vector<Date> ToDateVector(cli::array<System::DateTime>^ in);
spi::MatrixData<Date> ToDateMatrix(cli::array<System::DateTime,2>^ in);

// datetime input conversion
DateTime ToDateTime(System::DateTime in);
std::vector<DateTime> ToDateTimeVector(cli::array<System::DateTime>^ in);
spi::MatrixData<DateTime> ToDateTimeMatrix(cli::array<System::DateTime,2>^ in);

// enumerated type input conversion
template<class E, typename T>
E ToEnum(T in)
{
    return E(E::Enum(in));
}

template<class E, typename T>
std::vector<E> ToEnumVector(cli::array<T>^ in)
{
    int size = in->Length;
    std::vector<E> out;

    for (int i = 0; i < size; ++i)
        out.push_back(E(E::Enum(in[i])));

    return out;
}

template<class E, typename T>
E ToObject(T^ in, bool optional, const char* name="")
{
    if (in == nullptr)
    {
        if (optional)
            return E();
        throw RuntimeError("nullptr provided for non-optional field %s", name);
    }

    if (!in->self)
        in->make_inner_delegate();

    return E(in->self);
}

template<class E, typename T>
std::vector<E> ToObjectVector(cli::array<T>^ in, bool optional, const char* name="")
{
    int size = in->Length;
    std::vector<E> out;

    for (int i = 0; i < size; ++i)
    {
        if (in[i] == nullptr)
        {
            if (optional)
                out.push_back(E());

            throw RuntimeError("nullptr provided for element [%d] of %s", i, name);
        }
        else
        {
            if (!in[i]->self)
                in[i]->make_inner_delegate();

            out.push_back(E(in[i]->self));
        }
    }

    return out;
}

template<class E, typename T>
::spi::MatrixData<E> ToObjectMatrix(cli::array<T,2>^ in, bool optional, const char* name="")
{
    int nr = in->GetLength(0);
    int nc = in->GetLength(1);

    ::spi::MatrixData<E> out(nr, nc);

    for (int i = 0; i < nr; ++i)
    {
        for (int j = 0; j < nc; ++j)
        {
            if (in[i, j] == nullptr)
            {
                if (optional)
                    out[i, j] = E();
                else
                    throw RuntimeError("nullptr provided for element [%d,%d] of %s",
                        i, j, name);
            }
            else
            {
                if (!in[i, j]->self)
                    in[i, j]->make_inner_delegate();

                out[i][j] = E(in[i, j]->self);
            }
        }
    }

    return out;
}

SPI_END_NAMESPACE

#endif
