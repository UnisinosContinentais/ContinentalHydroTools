#include "continental/hydrotools/Cell.h"
#include "continental/hydrotools/CellWatershed.h"
#include <cmath>
#include <stdexcept>
#include <string>

namespace continental
{
namespace hydrotools
{
namespace service
{

CellWatershed::CellWatershed(double latitude, double longitude, size_t rows, size_t cols, double cellSize, double xllCorner, double yllCorner, size_t attribute) : Cell(0, 0)
{
    reset(latitude, longitude, rows, cols, cellSize, xllCorner, yllCorner, attribute);
}

void CellWatershed::convertCoordToRowCol(double latitude, double longitude, size_t rows, size_t cols, double cellSize, double xllCorner, double yllCorner)
{
    // Transformação das coordenadas do ponto em linhas e colunas
    // Lembrando que a posição (1,1) equivale a (0,0)

    // Calcula a coordenada X do centro da célula no canto esquerdo
    double xllCenter = xllCorner + (cellSize / 2);
    // Calcula a coordenada Y do centro da célula no canto esquerdo
    double yllCenter = yllCorner + (cellSize / 2);

    int xTemp = static_cast<int>(std::round((longitude - xllCenter) / cellSize));
    int yTemp = static_cast<int>(std::round((rows - 1) - (latitude - yllCenter) / cellSize));
    if (xTemp < 0 || xTemp > static_cast<int>(cols))
    {
        throw std::invalid_argument("Longitude " + std::to_string(longitude) + "º is outside the boundaries from the DEM.");
    }
    if (yTemp < 0 || yTemp > static_cast<int>(rows))
    {
        throw std::invalid_argument("Latitude " + std::to_string(latitude) + "º is outside the boundaries from the DEM.");
    }

    x = static_cast<size_t>(xTemp);
    y = static_cast<size_t>(yTemp);
}

void CellWatershed::reset(double latitude, double longitude, size_t rows, size_t cols, double cellSize, double xllCorner, double yllCorner, size_t attribute)
{
    Cell(0, 0);
    m_attribute = attribute;
    m_latitude = latitude;
    m_longitude = longitude;

    // Converte a coordenada para linha e coluna
    convertCoordToRowCol(latitude, longitude, rows, cols, cellSize, xllCorner, yllCorner);
}

}
}
}
