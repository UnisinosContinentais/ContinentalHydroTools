/*
* Developed by UNISINOS
* author: Luiz Felipe Bertoldi de Oliveira
* email: lbertoldio@unisinos.br
* date: January, 2019
*/

#ifndef CONTINENTAL_HYDROTOOLS_FLOWDIRECTION_H
#define CONTINENTAL_HYDROTOOLS_FLOWDIRECTION_H

#include <QString>
#include <vector>
#include <cmath>
#include <stdexcept>
#include <memory>
#include <continental/datamanagement/Raster.h>
#include "continental/hydrotools/HeuristicSinkRemovalUtil.h"

//*******************************************************************
//DIREÇÕES DE FLUXO (com base na codificação IDRISI)
//*******************************************************************

namespace continental
{
namespace hydrotools
{
/// <summary>
/// Calcula as direções de fluxo a partir de um MDE sem depressões
/// </summary>
/// <remarks></remarks>

template <class T>
class FlowDirection
{
private:
// Rasters
std::shared_ptr<datamanagement::Raster<T>> m_dem;
std::shared_ptr<datamanagement::Raster<short>> m_flowDirection;
// Matriz dos 8 vizinhos
float m_matrixD8[8] = {0, 0, 0, 0, 0, 0, 0, 0};
size_t m_countDepressions = 0;

public:
FlowDirection() = default;

/// <summary>
/// Retorna o MDE original ou modificado pelo processo
/// </summary>
std::shared_ptr<datamanagement::Raster<T>> getMDE()
{
    return m_dem;
}

/// <summary>
/// Retorna o Flow Direction
/// </summary>
std::shared_ptr<datamanagement::Raster<short>> getFlowDirection()
{
    return m_flowDirection;
}

//Lê um arquivo de MDE, especificando o local
void setMDE(std::shared_ptr<datamanagement::Raster<T>> mde)
{
    m_dem = mde;
    this->m_flowDirection = std::make_shared<datamanagement::Raster<short>>(m_dem->getRows(), m_dem->getCols(), m_dem->getXOrigin(), m_dem->getYOrigin(), m_dem->getCellSize(), m_dem->getNoDataValue());
}

//Identifica as direções de fluxo e as depressões espúrias do MDE
void calculateFlowDirection()
{
    flowDirectionAtBounds();

    bool FDFound = false;

    //Calcula para toda a grade as direções de fluxo
    size_t tempVar = m_dem->getRows() - 2;
    for (size_t y = 1; y <= tempVar; ++y)
    {
        size_t tempVar2 = m_dem->getCols() - 2;
        for (size_t x = 1; x <= tempVar2; ++x)
        {
            m_flowDirection->setData(y, x, incipientFlowDirection(y, x));

            //Se for = 0, verifica se existe uma célula com igual cota para onde possa verter
            if (m_flowDirection->getData(y, x) == 0)
            {
                verifyFlowDirAtBounds(y, x, FDFound);

                if (!FDFound) // Se não encontrar célula para verter água, identifica uma depressão
                {
                    m_countDepressions += 1;
                }

                FDFound = false;
            }
        }
    }
}

private:
//Verifica se algum vizinho de mesma cota, ou inferior já possui flow direction, atribuindo a mesma em caso verdadeiro
void verifyFlowDirAtBounds(size_t yc, size_t xc, bool &validated)
{

    //na openlist:,         'direções de apontamento para a célula central
    //For y = -1 To 1       2  4  8
    //posY = (Yc + y)       1    x   16
    //For x = -1 To 1       128   64  32
    //posX = (xc + x)

    validated = false;
    short elevation = m_dem->getData(yc, xc);
    int yi = 0;
    int xi = 0;

    for (int j = -1; j <= 1; ++j)
    {
        yi = static_cast<int>(yc) + j;
        for (int i = -1; i <= 1; ++i)
        {
            xi = static_cast<int>(xc) + i;

            //Exclui a célula central
            if (i != 0 || j != 0)
            {
                //Somente se a célula possui cota igual àquela sendo analisada
                if (m_dem->getData(static_cast<size_t>(yi), static_cast<size_t>(xi)) == elevation)
                {
                    if (m_flowDirection->getData(static_cast<size_t>(yi), static_cast<size_t>(xi)) == 0)
                    {
                        validated = true;
                    }

                    //caso tenha sido encontrado algum vizinho com direção de fluxo atribuída, manda água pra ele
                    if (validated)
                    {
                        m_flowDirection->setData(yc, xc, HeuristicSinkRemovalUtil::relativeIncipientFlowDirection(static_cast<int>(xc), static_cast<int>(xi), static_cast<int>(yc), static_cast<int>(yi)));
                        return;
                    }
                }
            }
        }
    }
}

//Calcula a Direção de fluxo nos limites da grade
void flowDirectionAtBounds()
{
    const size_t zero = 0;

    //Define o Flow direction dos cantos
    m_flowDirection->setData(zero, zero, m_flowDirection->getNoDataValue());
    m_flowDirection->setData(zero, m_dem->getCols() - 1, m_flowDirection->getNoDataValue());
    m_flowDirection->setData(m_dem->getRows() - 1, zero, m_flowDirection->getNoDataValue());
    m_flowDirection->setData(m_dem->getRows() - 1, m_dem->getCols() - 1, m_flowDirection->getNoDataValue());

    //Atribui o flow direction para fora dos limites da grade
    auto limitRows = static_cast<int>(m_dem->getRows() - 2);
    auto fixedColumn = m_dem->getCols() - 1;

    #pragma omp parallel for
    for (int y = 1; y <= limitRows; ++y)
    {
        m_flowDirection->setData(static_cast<size_t>(y), zero, m_flowDirection->getNoDataValue());
        m_flowDirection->setData(static_cast<size_t>(y), fixedColumn, m_flowDirection->getNoDataValue());
    }

    auto limitCols = static_cast<int>(m_dem->getCols() - 2);
    auto fixedRows = m_dem->getRows() - 1;

    #pragma omp parallel for
    for (int x = 1; x <= limitCols; ++x)
    {
        m_flowDirection->setData(zero, static_cast<size_t>(x), m_flowDirection->getNoDataValue());
        m_flowDirection->setData(fixedRows, static_cast<size_t>(x), m_flowDirection->getNoDataValue());
    }
}

//Retorna a direção de fluxo de uma célula qualquer, posicionada em x,y; Função das declividades das células vizinhas
short incipientFlowDirection(size_t y, size_t x)
{
    auto value = m_dem->getData(y, x);
    // Retorna o value do NODATA caso encontrá-lo
    if (value == m_dem->getNoDataValue())
    {
        return static_cast<short>(m_dem->getNoDataValue());
    }

    //Matriz D8 - 8 vizinhos:
    //0  1  2
    //3  x  4
    //5  6  7

    //Calcula o value da declividade para cada um dos vizinhos
    //Aquele que tiver a maior declividade é para onde irá a direção de fluxo
    //Para os sentidos 0, 2, 5 e 7 o comprimento é igual a Raiz(2)
    //Para os sentidos 1, 3, 4 e 6 o comprimento é igual a 1

    auto sqrt2 = static_cast<float>(std::sqrt(2));
    m_matrixD8[0] = (value - m_dem->getData(y - 1, x - 1)) / sqrt2;
    m_matrixD8[1] = value - m_dem->getData(y - 1, x);
    m_matrixD8[2] = (value - m_dem->getData(y - 1, x + 1)) / sqrt2;
    m_matrixD8[3] = value - m_dem->getData(y, x - 1);
    m_matrixD8[4] = value - m_dem->getData(y, x + 1);
    m_matrixD8[5] = (value - m_dem->getData(y + 1, x - 1)) / sqrt2;
    m_matrixD8[6] = value - m_dem->getData(y + 1, x);
    m_matrixD8[7] = (value - m_dem->getData(y + 1, x + 1)) / sqrt2;

    //32	64	    128
    //16	0	    1      'Configuração das direções de fluxo
    //8	    4	    2

    float max = 0.01F;
    // value inicial que indica que não há direção de fluxo
    short enumMax = -1;

    // Seleciona a maior declividade dentre as calculadas
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
};
}
}

#endif // CONTINENTAL_HYDROTOOLS_FLOWDIRECTION_H