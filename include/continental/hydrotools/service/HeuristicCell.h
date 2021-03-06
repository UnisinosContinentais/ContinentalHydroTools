﻿/*
* Developed by UNISINOS
* author: Luiz Felipe Bertoldi de Oliveira
* email: lbertoldio@unisinos.br
* date: January, 2019
*/

#ifndef CONTINENTAL_HYDROTOOLS_HEURISTICCELL_H
#define CONTINENTAL_HYDROTOOLS_HEURISTICCELL_H

#include <cmath>
#include <stdexcept>
#include "continental/hydrotools/service/Cell.h"

//*******************************************************************
//CLASSES ESPECÍFICAS DE CÉLULAS UTILIZADAS PELO PROGRAMA
//*******************************************************************

namespace continental
{
namespace hydrotools
{
namespace service
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
		bool valid = false;

        HeuristicCell(size_t row, size_t column);

        HeuristicCell();
};

}
}
}

#endif // CONTINENTAL_HYDROTOOLS_HEURISTICCELL_H
