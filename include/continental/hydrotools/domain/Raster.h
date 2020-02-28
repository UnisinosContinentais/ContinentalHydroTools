/*
* Developed by UNISINOS - Projeto Continentais and Petrobras
* author: Luiz Felipe Bertoldi de Oliveira
* email: lbertoldio@unisinos.br
* date: June, 2019
*/
#ifndef RASTER_H
#define RASTER_H

#include <algorithm>
#include <vector>
#include <assert.h>


namespace continental {
namespace hydrotools {
namespace domain {


template <class T>
class Raster
{
private:
    // Numero Colunas
    size_t m_cols = 0;
    // Numero Linhas
    size_t m_rows = 0;
    double m_x_origin = 0;
    double m_y_origin = 0;
    // Resolução do raster
    double m_cellSize = 0;
    // Valor do NODATA
    T m_noDataValue = 0;

    bool m_modified = true;
    bool m_hasPersisted = false;
    unsigned short m_version = 0;

    std::vector<T> m_data;
    T m_maxValue;
public:
    /// Construtor
    Raster() : m_maxValue(std::numeric_limits<T>::min())
    {
        static_assert(std::is_same<T, short>::value || std::is_same<T, float>::value, "Raster only accepts short or float.");
    }

    Raster(size_t rows, size_t cols, double xOrigin, double yOrigin, double cellSize, int noData) : Raster()
    {
        reset(rows, cols, xOrigin, yOrigin, cellSize, noData);
    }

    /// <summary>
    /// Retorna o número de linhas
    /// </summary>
    inline size_t getRows() const
    {
        return m_rows;
    }

    /// <summary>
    /// Retorna o número de colunas
    /// </summary>
    inline size_t getCols() const
    {
        return m_cols;
    }

    /// <summary>
    /// Retorna a resolução da grade
    /// </summary>
    inline double getCellSize() const
    {
        return m_cellSize;
    }

    /// <summary>
    /// Retorna o valor para o qual não se tem dados no raster
    /// </summary>
    inline T getNoDataValue() const
    {
        return m_noDataValue;
    }

    /// <summary>
    /// Retorna o número total de células
    /// </summary>
    inline size_t getTotalCells() const
    {
        return m_cols * m_rows;
    }

    /// <summary>
    /// Retorna a coordenada X do canto superior da grade
    /// </summary>
    inline double getXOrigin() const
    {
        return m_x_origin;
    }

    /// <summary>
    /// Retorna a coordenada Y do canto superior da grade
    /// </summary>
    inline double getYOrigin() const
    {
        return m_y_origin;
    }

    /// <summary>
    /// Retorna o máximo valor dos dados da matriz(y, x) do raster
    /// </summary>
    inline T calculateMaxValue() const
    {
        auto maxValue = m_data[0];
        for (auto item : m_data)
        {
            maxValue = std::max(maxValue, item);
        }
        return maxValue;
    }

    inline T getData(const size_t position) const
    {
        assert(position < m_data.size());

        return m_data[position];
    }

    inline T getData(const size_t row, const size_t column) const
    {
        return getData(row * m_cols + column);
    }

    inline T getData(const int row, const int column) const
    {
        return getData(static_cast<size_t>(row), static_cast<size_t>(column));
    }

    inline void setData(const size_t position, const T value)
    {
        assert(position < m_data.size());

        m_data[position] = value;
    }

    inline void setData(const size_t row, const size_t column, const T value)
    {
        setData(row * m_cols + column, value);
    }

    inline const std::vector<T>& getData() const
    {
        return m_data;
    }

    inline void reset(size_t rows, size_t cols, double xOrigin, double yOrigin, double cellSize, int noData)
    {
        m_cols = cols;
        m_rows = rows;
        m_x_origin = xOrigin;
        m_y_origin = yOrigin;
        m_cellSize = cellSize;
        m_noDataValue = noData;
        m_data.clear();
        m_data.resize(rows * cols);
    }

    inline bool getModified() const
    {
        return m_modified;
    }

    inline void setModified(bool modified)
    {
        m_modified = modified;
    }

    inline bool isPersisted() const
    {
        return m_hasPersisted;
    }

    inline void setPersisted(bool hasPersisted)
    {
        m_hasPersisted = hasPersisted;
    }

    inline unsigned short getVersion() const
    {
        return m_version;
    }

    inline void setVersion(unsigned short version)
    {
        m_version = version;
    }
};

} //fluvialsystem
} //domain
} //continental
#endif // RASTER_H
