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
#ifndef SPI_CLI_OUTPUT_HPP
#define SPI_CLI_OUTPUT_HPP

/*
***************************************************************************
** cliOutput.hpp
**
** Functions which convert from standard and spi-specific c++ data types
** to various native C++/CLI data types.
**
** Each function has a scalar and a vector equivalent.
***************************************************************************
*/

#using <mscorlib.dll>

#include <spi/Date.hpp>
#include <spi/DateTime.hpp>
#include <spi/MatrixData.hpp>

#include <string>
#include <vector>

SPI_BEGIN_NAMESPACE

// boolean output conversion
System::Boolean FromBool(bool in);
cli::array<System::Boolean>^ FromBoolVector(const std::vector<bool>& in);
cli::array<System::Boolean,2>^ FromBoolMatrix(const ::spi::MatrixData<bool>& in);

// integer output conversion
System::Int32 FromInt(int in);
cli::array<System::Int32>^ FromIntVector(const std::vector<int>& in);
cli::array<System::Int32,2>^ FromIntMatrix(const ::spi::MatrixData<int>& in);

//System::Int32 FromLong(long in);
//cli::array<System::Int32>^ FromLongVector(const std::vector<long>& in);

// unsigned integer output conversion
//System::UInt32 FromSizeT(size_t in);
//cli::array<System::UInt32>^ FromSizeTVector(const std::vector<size_t>& in);

// floating point output conversion
System::Double FromDouble(double in);
cli::array<System::Double>^ FromDoubleVector(const std::vector<double>& in);
cli::array<System::Double,2>^ FromDoubleMatrix(const ::spi::MatrixData<double>& in);

// string output conversion
System::String^ FromString(const std::string& in);
cli::array<System::String^>^ FromStringVector(const std::vector<std::string>& in);
cli::array<System::String^,2>^ FromStringMatrix(const ::spi::MatrixData<std::string>& in);

// character output conversion
System::SByte FromChar(char in);
cli::array<System::SByte>^ FromCharVector(const std::vector<char>& in);

// date output conversion
System::DateTime FromDate(Date in);
cli::array<System::DateTime>^ FromDateVector(const std::vector<Date>& in);
cli::array<System::DateTime,2>^ FromDateMatrix(const ::spi::MatrixData<Date>& in);

// datetime output conversion
System::DateTime FromDateTime(DateTime in);
cli::array<System::DateTime>^ FromDateTimeVector(const std::vector<DateTime>& in);
cli::array<System::DateTime,2>^ FromDateTimeMatrix(const ::spi::MatrixData<DateTime>& in);

// enumerated type input conversion
template<typename T, class E>
T FromEnum(E in)
{
    return T(E::Enum(in));
}

template<typename T, class E>
cli::array<T>^ FromEnumVector(const std::vector<E>& in)
{
    int size = spi_util::IntegerCast<int>(in.size());

    cli::array<T>^ out = gcnew cli::array<T>(size);

    for (int i = 0; i < size; ++i)
        out[i] = T(E::Enum(in[i]));

    return out;
}

template<typename T, class E>
cli::array<T^>^ FromObjectVector(const std::vector<E>& in)
{
    int size = spi_util::IntegerCast<int>(in.size());

    cli::array<T^>^ out = gcnew cli::array<T^>(size);

    for (int i = 0; i < size; ++i)
        out[i] = T::Wrap(in[i].get());

    return out;
}

template<typename T, class E>
cli::array<T^,2>^ FromObjectMatrix(const spi::MatrixData<E>& in)
{
    int nr = spi_util::IntegerCast<int>(in.Rows());
    int nc = spi_util::IntegerCast<int>(in.Cols());

    cli::array<T^,2>^ out = gcnew cli::array<T^,2>(nr, nc);

    for (int i = 0; i < nr; ++i)
        for (int j = 0; j < nc; ++j)
            out[i,j] = T::Wrap(in[i][j].get());

    return out;
}

SPI_END_NAMESPACE

#endif
