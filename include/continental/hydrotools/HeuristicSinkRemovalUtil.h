#ifndef CONTINENTAL_HYDROTOOLS_HEURISTICSINKREMOVALUTIL_H
#define CONTINENTAL_HYDROTOOLS_HEURISTICSINKREMOVALUTIL_H

namespace continental
{
namespace hydrotools
{
class HeuristicSinkRemovalUtil
{
    public:
        //Retorna o indicador da direção de fluxo comparando duas células adjacentes
        static short relativeIncipientFlowDirection(size_t x1, size_t x2, size_t y1, size_t y2);

        //Move a linha e a coluna de acordo com o número armazenado
        static void moveToFlowDirection(short value, int &row, int &column);
};
}
}

#endif // CONTINENTAL_HYDROTOOLS_HEURISTICSINKREMOVALUTIL_H
