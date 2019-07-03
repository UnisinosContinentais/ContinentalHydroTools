﻿#ifndef CONTINENTAL_HYDROTOOLS_CELLS_H
#define CONTINENTAL_HYDROTOOLS_CELLS_H

#include <cstddef>

//*******************************************************************
//CLASSES ESPECÍFICAS DE CÉLULAS UTILIZADAS PELO PROGRAMA
//*******************************************************************

namespace continental
{
namespace hydrotools
{
/// <summary>
/// Classe base da célula, para indicar a posição na matriz
/// </summary>
class Cell
{
    public:
        size_t x = 0;
        size_t y = 0;

        Cell(size_t x, size_t y);

        Cell() = default;
};
}
}

#endif // CONTINENTAL_HYDROTOOLS_CELLS_H
