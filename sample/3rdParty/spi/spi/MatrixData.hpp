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
** MatrixData.hpp
***************************************************************************
** MatrixData template classes - for regular 2D arrays.
***************************************************************************
*/

#ifndef SPI_MATRIX_DATA_HPP
#define SPI_MATRIX_DATA_HPP

#include "Namespace.hpp"
#include "RuntimeError.hpp"
#include "RefCounter.hpp"
#include "Map.hpp"
#include "Value.hpp"

#include <vector>
#include <spi_boost/shared_array.hpp>
#include <spi_util/MatrixData.hpp>

SPI_BEGIN_NAMESPACE

SPI_DECLARE_RC_CLASS(Map);

template <typename T>
class MatrixData : public spi_util::MatrixData<T>
{
public:
    // constructor of an empty zero size matrix
    MatrixData()
        :
        spi_util::MatrixData<T>()
    {}

    // constructor of an empty non-zero size matrix
    // subsequently you need to fill it element-by-element
    // values within the matrix initially correspond to T()
    MatrixData(size_t nbRows, size_t nbCols)
        :
        spi_util::MatrixData<T>(nbRows, nbCols)
    {}

    // constructor of a non-zero size matrix using a 1D-vector
    // to initialise the data
    //
    // the 1D-vector is row-by-row - thus 4 rows, 3 columns the first
    // 3-elements are row zero, the next 3-elements are row one etc.
    MatrixData(
        size_t nbRows, size_t nbCols,
        const std::vector<T>& data)
        :
        spi_util::MatrixData<T>(nbRows, nbCols, data)
    {}

    MatrixData(const spi_util::MatrixData<T>& matrixData)
        :
    spi_util::MatrixData<T>(matrixData)
    {}

#if 0
    // converts ToMap for serialization purposes
    MapConstSP ToMap() const
    {
        MapSP m(new Map(Map::MATRIX));

        m->SetValue("rows", (int)this->Rows());
        m->SetValue("cols", (int)this->Cols());
        m->SetValue("data", this->Data());

        return m;
    }
#endif

    // constructor FromMap for de-serialization purposes
    //
    // note that we no longer encode a matrix via a map, but for backward
    // compatibility we need to support this format
    static MatrixData FromMap(const MapConstSP& m)
    {
        bool permissive = m->Permissive();
        size_t nbRows = m->GetValue("rows").getInt(permissive);
        size_t nbCols = m->GetValue("cols").getInt(permissive);
        const std::vector<T>& data = m->GetValue("data").getVector<T>(permissive);

        return MatrixData(nbRows, nbCols, data);
    }

    // converts ToArray for serialization purposes
    IArrayConstSP ToArray() const
    {
        std::vector<size_t> dims;

        dims.push_back(spi_util::MatrixData<T>::Rows());
        dims.push_back(spi_util::MatrixData<T>::Cols());

        return IArrayConstSP(new Array<T>(spi_util::MatrixData<T>::Data(), dims));
    }

    // constructor FromValueMatrix for de-serialization purposes
    static MatrixData FromValueMatrix(const MatrixData<Value>& vmat)
    {
        return MatrixData(vmat.Rows(),
            vmat.Cols(),
            ConvertValueVector<T>(vmat.Data()));
    }

    // cast to Value for translation purposes
    operator Value() const
    {
        return Value(ToArray());
    }

    // constructor FromValue for translation purposes
    static MatrixData FromValue(const Value& v)
    {
        if (v.getType() == Value::MAP)
        {
            MapConstSP aMap = v.getMap();
            return FromMap(aMap);
        }

        return FromValueMatrix(v.getMatrix());
    }

};

SPI_END_NAMESPACE

#endif
