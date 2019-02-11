#ifndef IPHYDRORASTERTOOLS_CELLS_H
#define IPHYDRORASTERTOOLS_CELLS_H

#include <cstddef>

//*******************************************************************
//CLASSES ESPECÍFICAS DE CÉLULAS UTILIZADAS PELO PROGRAMA
//Criado por Vinícius Alencar Siqueira - 20/01/2014
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
};
}
}

#endif // IPHYDRORASTERTOOLS_CELLS_H
