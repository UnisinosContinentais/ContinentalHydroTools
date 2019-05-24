﻿#ifndef CONTINENTAL_HYDROTOOLS_CELLWATERSHED_H
#define CONTINENTAL_HYDROTOOLS_CELLWATERSHED_H

#include "continental/hydrotools/Cell.h"

//*******************************************************************
//CLASSES ESPECÍFICAS DE CÉLULAS UTILIZADAS PELO PROGRAMA
//*******************************************************************

namespace continental
{
namespace hydrotools
{
/// <summary>
/// Classe que armazena as células com os exutórios das bacias
/// </summary>
/// <remarks></remarks>
class CellWatershed : public Cell
{
public:
    size_t m_attribute = 0;
    double m_latitude = 0;
    double m_longitude = 0;

    CellWatershed(double latitude, double longitude, size_t rows, size_t cols, double cellSize, double xOrigin, double yOrigin, size_t atrib);
    /// <summary>
    /// Converte a coordenada geográfica para linha e coluna
    /// </summary>
    void convertCoordToRowCol(double latitude, double m_longitude, size_t rows, size_t cols, double cellSize, double xOrigin, double yOrigin);

    size_t getAttribute()
    {
        return m_attribute;
    }

    void setAttribute(size_t attribute)
    {
        m_attribute = attribute;
    }
private:
    void reset(double latitude, double longitude, size_t rows, size_t cols, double cellSize, double xOrigin, double yOrigin, size_t atrib);
};
}
}

#endif // CONTINENTAL_HYDROTOOLS_CELLWATERSHED_H
