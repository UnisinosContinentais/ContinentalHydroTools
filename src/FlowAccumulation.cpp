#include "continental/hydrotools/FlowAccumulation.h"
#include "continental/hydrotools/FlowDirection.h"
#include "continental/hydrotools/HeuristicSinkRemovalUtil.h"
#include <continental/dataManagement/RasterFile.h>

namespace continental
{
namespace hydrotools
{
Raster<float> FlowAccumulation::getFlowAccumulationMatrix()
{
    return m_flowAccumulation;
}

Raster<short> FlowAccumulation::getFlowDirectionMatrix()
{
    return *m_flowDirection;
}

void FlowAccumulation::setFlowDirection(shared_ptr<Raster<short>> flowDirection)
{
    m_flowDirection = flowDirection;
    m_flowAccumulation = Raster<float>(m_flowDirection->getRows(), m_flowDirection->getCols(), m_flowDirection->getXOrigin(), m_flowDirection->getYOrigin(), m_flowDirection->getCellSize(), m_flowDirection->getNoDataValue());
}

FlowAccumulation::FlowAccumulation()
{
    this->m_flowDirection = make_shared<Raster<short>>();
}

void FlowAccumulation::readFlowDirection(const QString &Local)
{
    m_flowDirection = make_shared<Raster<short>>(RasterFile<short>::loadRasterByFile(Local));
    m_flowAccumulation = Raster<float>(m_flowDirection->getRows(), m_flowDirection->getCols(), m_flowDirection->getXOrigin(), m_flowDirection->getYOrigin(), m_flowDirection->getCellSize(), m_flowDirection->getNoDataValue());
}

void FlowAccumulation::runoff()
{
    int posX = 0, posY = 0;

    m_checkedNodeList.resize(m_flowDirection->getTotalCells());

    for (int i = 0; i < static_cast<int>(m_flowDirection->getRows()); i++)
    {
        for (int j = 0; j < static_cast<int>(m_flowDirection->getCols()); j++)
        {
            posX = j;
            posY = i;

            // Se a célula ainda não foi checada
            auto posCheckedNodeList = static_cast<size_t>(posY) + static_cast<size_t>(posX) * m_flowDirection->getCols();
            if (m_checkedNodeList[posCheckedNodeList] == false)
            {
                float totalCells = 0;

                while (true)
                {
                    // Verifica se todas as células vwizinhas já foram analizadas; Caso sim:
                    if (neighbourCellsAnalyzed(posX, posY) == false)
                        break;

                    // Armazena o número de células acumuladas
                    totalCells = m_flowAccumulation.getData(static_cast<size_t>(posY), static_cast<size_t>(posX));
                    //Indica que a célula foi analizada
                    m_checkedNodeList[posCheckedNodeList] = true;
                    // Move na direção do flow direction
                    size_t posYTemp = static_cast<size_t>(posY), posXTemp = static_cast<size_t>(posX);
                    HeuristicSinkRemovalUtil::moveToFlowDirection(m_flowDirection->getData(static_cast<size_t>(posY), static_cast<size_t>(posX)), posYTemp, posXTemp);

                    //Se extrapolar os limites da matriz, sai do loop
                    if (posX < 0 || posY < 0 || posX > static_cast<int>(m_flowDirection->getCols() - 1) || posY > static_cast<int>(m_flowDirection->getRows() - 1))
                        break;

                    if (m_flowDirection->getData(static_cast<size_t>(posY), static_cast<size_t>(posX)) == m_flowDirection->getNoDataValue()) //Se chegar em um NODATA, sai do loop
                    {
                        m_flowAccumulation.setData(static_cast<size_t>(posY), static_cast<size_t>(posX), m_flowDirection->getNoDataValue());
                        break;
                    }

                    // Acumula as células anteriores
                    m_flowAccumulation.setData(static_cast<size_t>(posY), static_cast<size_t>(posX), m_flowAccumulation.getData(static_cast<size_t>(posY), static_cast<size_t>(posX)) + (totalCells + 1));
                }
            }
        }
    }

}

bool FlowAccumulation::neighbourCellsAnalyzed(int xc, int yc)
{
    //na openlist:,         'direções de apontamento para a célula central
    //For y = -1 To 1       2  4  8
    //posY = (Yc + y)       1    x   16
    //For x = -1 To 1       128   64  32
    //posX = (xc + x)

    int xi = 0;
    int yi = 0;
    bool cellContribution = false;

    for (short y = -1; y <= 1; y++)
    {
        yi = yc + y;
        for (short x = -1; x <= 1; x++)
        {
            xi = xc + x;

            // Evita a análise da própria célula (nó central)
            if (x == 0 && y == 0)
                continue;

            // Evita sair dos limites do raster
            if (xi < 0 || yi < 0 || static_cast<size_t>(xi) >= m_flowDirection->getCols() || static_cast<size_t>(yi) >= m_flowDirection->getRows())
                continue;

            // Se houver uma das células vizinhas apontando para o centro, aciona o flag
            if (HeuristicSinkRemovalUtil::relativeIncipientFlowDirection(static_cast<size_t>(xi), static_cast<size_t>(xc), static_cast<size_t>(yi), static_cast<size_t>(yc)) == m_flowDirection->getData(static_cast<size_t>(yi), static_cast<size_t>(xi)))
                cellContribution = true;

            // Se a célula vizinha estiver apontando para o centro, mas não tiver sido checada ainda, retorna falso.
            if (cellContribution == true && m_checkedNodeList[static_cast<size_t>(yi) + static_cast<size_t>(xi) * m_flowDirection->getCols()] == false)
                return false;

            // Reseta para verificar a próxima célula
            cellContribution = false;
        }
    }

    //Se passar por todas as células, e nenhuma contribuir ou todas já tiverem sido checadas então
    return true;
}
}
}
