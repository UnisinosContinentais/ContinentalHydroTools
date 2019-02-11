#ifndef IPHYDRORASTERTOOLS_CELLWATERSHED_H
#define IPHYDRORASTERTOOLS_CELLWATERSHED_H

#include "continental/hydrotools/Cell.h"

//*******************************************************************
//CLASSES ESPECÍFICAS DE CÉLULAS UTILIZADAS PELO PROGRAMA
//Criado por Vinícius Alencar Siqueira - 20/01/2014
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
    short m_attribute = 0;
    double m_latitude = 0;
    double m_longitude = 0;

    CellWatershed(double latitude, double longitude, int rows, int cols, double cellSize, double xOrigin, double yOrigin, short atrib);
    /// <summary>
    /// Converte a coordenada geográfica para linha e coluna
    /// </summary>
    void convertCoordToRowCol(double latitude, double m_longitude, int rows, int cols, double cellSize, double xOrigin, double yOrigin);
};
}
}

#endif // IPHYDRORASTERTOOLS_CELLWATERSHED_H
