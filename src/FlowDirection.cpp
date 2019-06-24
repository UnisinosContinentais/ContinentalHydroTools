#include "continental/hydrotools/FlowDirection.h"
#include "continental/hydrotools/HeuristicSinkRemovalUtil.h"

#include <continental/datamanagement/RasterFile.h>

using namespace std;
using namespace continental::datamanagement;

namespace continental
{
namespace hydrotools
{

Raster<short> FlowDirection::getMDEMatrix()
{
    return *m_MDE;
}

Raster<short> FlowDirection::getFlowDirection()
{
    return *m_flowDirection;
}

void FlowDirection::readMDEdata(const QString &filepath)
{
    m_MDE = make_unique<Raster<short>>(RasterFile<short>::loadRasterByFile(filepath));
    this->m_flowDirection = make_unique<Raster<short>>(m_MDE->getRows(), m_MDE->getCols(), m_MDE->getXOrigin(), m_MDE->getYOrigin(), m_MDE->getCellSize(), m_MDE->getNoDataValue());
}

void FlowDirection::calculateFlowDirection()
{
    bool FDFound = false;

    //Calcula para toda a grade as direções de fluxo
    size_t tempVar = m_MDE->getRows() - 2;
    for (size_t y = 1; y <= tempVar; ++y)
    {
        size_t tempVar2 = m_MDE->getCols() - 2;
        for (size_t x = 1; x <= tempVar2; ++x)
        {
            m_flowDirection->setData(x, y, incipientFlowDirection(x, y));

            //Se for = 0, verifica se existe uma célula com igual cota para onde possa verter
            if (m_flowDirection->getData(x, y) == 0)
            {
                verifyFlowDirAtBounds(x, y, FDFound);

                if (FDFound == false) // Se não encontrar célula para verter água, identifica uma depressão
                {
                    m_countDepressions += 1;
                }

                FDFound = false;
            }
        }
    }

}

void FlowDirection::verifyFlowDirAtBounds(size_t xc, size_t yc, bool &validated)
{

    //na openlist:,         'direções de apontamento para a célula central
    //For y = -1 To 1       2  4  8
    //posY = (Yc + y)       1    x   16
    //For x = -1 To 1       128   64  32
    //posX = (xc + x)

    validated = false;
    short elevation = m_MDE->getData(xc, yc);
    int yi = 0;
    int xi = 0;

    for (int j = -1; j <= 1; ++j)
    {
        yi = static_cast<int>(yc) + j;
        for (int i = -1; i <= 1; ++i)
        {
            xi = static_cast<int>(xc) + i;

            if (i != 0 || j != 0) //Exclui a célula central
            {

                if (m_MDE->getData(static_cast<size_t>(xi), static_cast<size_t>(yi)) == elevation) //Somente se a célula possui cota igual àquela sendo analisada
                {
                    if (m_flowDirection->getData(static_cast<size_t>(xi), static_cast<size_t>(yi)) == 0)
                    {
                        validated = true;
                    }

                    if (validated == true) //caso tenha sido encontrado algum vizinho com direção de fluxo atribuída, manda água pra ele
                    {
                        m_flowDirection->setData(xc, yc, HeuristicSinkRemovalUtil::relativeIncipientFlowDirection(static_cast<int>(xc), static_cast<int>(xi), static_cast<int>(yc), static_cast<int>(yi)));
                        return;
                    }

                }
            }
        }
    }
}

void FlowDirection::flowDirectionAtBounds()
{
    size_t zero = static_cast<size_t>(0);

    //Define o Flow direction dos cantos
    m_flowDirection->setData(0, 0, 32);
    m_flowDirection->setData(zero, m_MDE->getCols() - 1, 128);
    m_flowDirection->setData(m_MDE->getRows() - 1, zero, 8);
    m_flowDirection->setData(m_MDE->getRows() - 1, m_MDE->getCols() - 1, 2);

    //Atribui o flow direction para fora dos limites da grade
    size_t tempVar = m_MDE->getRows() - 2;
    for (size_t y = 1; y <= tempVar; ++y)
    {
        m_flowDirection->setData(y, zero, 16);
    }

    size_t tempVar2 = m_MDE->getRows() - 2;
    for (size_t y = 1; y <= tempVar2; ++y)
    {
        m_flowDirection->setData(y, m_MDE->getCols() - 1, 1);
    }

    size_t tempVar3 = m_MDE->getCols() - 2;
    for (size_t x = 1; x <= tempVar3; ++x)
    {
        m_flowDirection->setData(zero, x, 64);
    }

    size_t tempVar4 = m_MDE->getCols() - 2;
    for (size_t x = 1; x <= tempVar4; ++x)
    {
        m_flowDirection->setData(m_MDE->getRows() - 1, x, 4);
    }

}

short FlowDirection::incipientFlowDirection(size_t x, size_t y)
{
    short valor = m_MDE->getData(x, y);
    if (valor == m_MDE->getNoDataValue())
    {
        return static_cast<short>(m_MDE->getNoDataValue());
    }

    float sqrt2 = static_cast<float>(std::sqrt(2));

    //Matriz D8 - 8 vizinhos:
    //0  1  2
    //3  x  4
    //5  6  7

    //Calcula o valor da declividade para cada um dos vizinhos
    //Aquele que tiver a maior declividade é para onde irá a direção de fluxo
    //Para os sentidos 0, 2, 5 e 7 o comprimento é igual a Raiz(2)
    //Para os sentidos 1, 3, 4 e 6 o comprimento é igual a 1
    m_matrixD8[0] = (valor - m_MDE->getData(x - 1, y - 1)) / sqrt2;
    m_matrixD8[1] = valor - m_MDE->getData(x, y - 1);
    m_matrixD8[2] = (valor - m_MDE->getData(x + 1, y - 1)) / sqrt2;
    m_matrixD8[3] = valor - m_MDE->getData(x - 1, y);
    m_matrixD8[4] = valor - m_MDE->getData(x + 1, y);
    m_matrixD8[5] = (valor - m_MDE->getData(x - 1, y + 1)) / sqrt2;
    m_matrixD8[6] = valor - m_MDE->getData(x, y + 1);
    m_matrixD8[7] = (valor - m_MDE->getData(x + 1, y + 1)) / sqrt2;

    //32	64	    128
    //16	0	    1      'Configuração das direções de fluxo para o IDRISI Kilimanjaro
    //8	4	    2

    float max = 0.01F;
    short enumMax = -1; //Valor inicial que indica que não há direção de fluxo

    //Seleciona a maior declividade dentre as calculadas
    for (short i = 0; i <= 7; ++i)
    {
        if (m_matrixD8[i] > max)
        {
            max = m_matrixD8[i];
            enumMax = i;
        }
    }

    // Identifica o sentido conforme o número do enumerador máximo
    switch (enumMax)
    {
        case 0:
            return 32;
        case 1:
            return 64;
        case 2:
            return 128;
        case 3:
            return 16;
        case 4:
            return 1;
        case 5:
            return 8;
        case 6:
            return 4;
        case 7:
            return 2;
        default:
            return 0;
    }
}

}
}
