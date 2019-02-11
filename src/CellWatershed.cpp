#include "continental/hydrotools/Cell.h"
#include "continental/hydrotools/CellWatershed.h"

#include <string>

namespace continental
{
namespace hydrotools
{
CellWatershed::CellWatershed(double latitude, double longitude, int rows, int cols, double cellSize, double xllCorner, double yllCorner, short attribute) :
    Cell(0, 0), m_attribute(attribute), m_latitude(latitude), m_longitude(longitude)
{
    // Converte a coordenada para linha e coluna
    convertCoordToRowCol(latitude, longitude, rows, cols, cellSize, xllCorner, yllCorner);
}

void CellWatershed::convertCoordToRowCol(double latitude, double longitude, int rows, int cols, double cellSize, double xllCorner, double yllCorner)
{
    // Transformação das coordenadas do ponto em linhas e colunas
    // Lembrando que a posição (1,1) equivale a (0,0)
    double xllCenter = 0;
    double yllCenter = 0;
    // Calcula a coordenada X do centro da célula no canto esquerdo
    xllCenter = xllCorner + (cellSize / 2);
    // Calcula a coordenada Y do centro da célula no canto esquerdo
    yllCenter = yllCorner + (cellSize / 2);

    int x = static_cast<int>(std::round((longitude - xllCenter) / cellSize));
    int y = static_cast<int>(std::round((rows - 1) - (latitude - yllCenter) / cellSize));
    if (x < 0 || x > cols)
    {
        throw std::invalid_argument("Longitude " + std::to_string(longitude) + "º is outside the boundaries from the DEM.");
    }
    if (y < 0 || y > rows)
    {
        throw std::invalid_argument("Latitude " + std::to_string(latitude) + "º is outside the boundaries from the DEM.");
    }

}
}
}
