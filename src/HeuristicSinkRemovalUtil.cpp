#include "continental/hydrotools/HeuristicSinkRemovalUtil.h"
#include <stdexcept>

namespace continental
{
namespace hydrotools
{

void HeuristicSinkRemovalUtil::moveToFlowDirection(short value, int &row, int &column)
{
    //32	64	    128
    //16	0	    1      'Configuração das direções de fluxo para o IDRISI Kilimanjaro
    //8	4	    2

    switch (value) //Identifica o sentido conforme o numero
    {
        case 128:
            row--;
            column++;
            break;
        case 1:
            column++;
            break;
        case 2:
            row++;
            column++;
            break;
        case 4:
            row++;
            break;
        case 8:
            row++;
            column--;
            break;
        case 16:
            column--;
            break;
        case 32:
            row--;
            column--;
            break;
        case 64:
            row--;
            break;
        default:
            //Não muda, pois encontrou uma borda/depressão
            break;
    }
}

short HeuristicSinkRemovalUtil::relativeIncipientFlowDirection(int x1, int x2, int y1, int y2)
{
    //célula 1 = célula de origem
    //célula 2 = célula de destino

    int xRel = x2 - x1;
    int yRel = y2 - y1;

    if (xRel == 1 && yRel == -1)
    {
        return 128;
    }
    if (xRel == 1 && yRel == 0)
    {
        return 1;
    }
    if (xRel == 1 && yRel == 1)
    {
        return 2;
    }
    if (xRel == 0 && yRel == 1)
    {
        return 4;
    }
    if (xRel == -1 && yRel == 1)
    {
        return 8;
    }
    if (xRel == -1 && yRel == 0)
    {
        return 16;
    }
    if (xRel == -1 && yRel == -1)
    {
        return 32;
    }
    if (xRel == 0 && yRel == -1)
    {
        return 64;
    }

    //Caso encontre algum outro valor dispara um exception
    throw std::runtime_error("Problems when trying to identify the flow direction from the modified river section.");
}

}
}
