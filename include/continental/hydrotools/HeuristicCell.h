#ifndef IPHYDRORASTERTOOLS_HEURISTICCELL_H
#define IPHYDRORASTERTOOLS_HEURISTICCELL_H

#include <cmath>
#include <stdexcept>
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
/// Celula com indicação do valor heuristico para algoritmo de remoção de depressões
/// </summary>
class HeuristicCell : public Cell
{
    public:
        float costFunction = 0;
        // posições relativas de X e Y, de onde a célula vem
        int relParentX = 0;
        int relParentY = 0;

        HeuristicCell(size_t row, size_t column);
};
}
}

#endif // IPHYDRORASTERTOOLS_HEURISTICCELL_H
