#include "continental/hydrotools/service/FlowAccumulation.h"
#include "continental/hydrotools/service/FlowDirection.h"
#include "continental/hydrotools/service/HeuristicSinkRemovalUtil.h"

using namespace std;
using namespace continental::datamanagement;

namespace continental
{
namespace hydrotools
{
namespace service
{


shared_ptr<Raster<float>> FlowAccumulation::getFlowAccumulation() const
{
    return m_flowAccumulation;
}

shared_ptr<Raster<short>> FlowAccumulation::getFlowDirection() const
{
    return m_flowDirection;
}

void FlowAccumulation::setFlowDirection(shared_ptr<Raster<short>> flowDirection)
{
    m_flowDirection = flowDirection;
    m_flowAccumulation = make_shared<Raster<float>>(m_flowDirection->getRows(), m_flowDirection->getCols(), m_flowDirection->getXOrigin(), m_flowDirection->getYOrigin(), m_flowDirection->getCellSize(), m_flowDirection->getNoDataValue());
}

FlowAccumulation::FlowAccumulation()
{
    this->m_flowDirection = make_shared<Raster<short>>();
}

FlowAccumulation::~FlowAccumulation()
{

}

void FlowAccumulation::runoff()
{
    m_checkedNodeList.resize(m_flowDirection->getTotalCells());

    auto rows = static_cast<int>(m_flowDirection->getRows());
    auto cols = static_cast<int>(m_flowDirection->getCols());
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            int posX = j;
            int posY = i;

            // Se a célula ainda não foi checada
            auto posCheckedNodeList = static_cast<size_t>(posY) * m_flowDirection->getCols() + static_cast<size_t>(posX);
            if (m_checkedNodeList[posCheckedNodeList] == false)
            {
                while (true)
                {
                    // Verifica se todas as células vwizinhas já foram analizadas; Caso sim:
                    if (neighbourCellsAnalyzed(posY, posX) == false)
                    {
                        break;
                    }

                    // Armazena o número de células acumuladas
                    float totalCells = m_flowAccumulation->getData(static_cast<size_t>(posY), static_cast<size_t>(posX));
                    //Indica que a célula foi analizada
                    m_checkedNodeList[posCheckedNodeList] = true;
                    // Move na direção do flow direction
                    HeuristicSinkRemovalUtil::moveToFlowDirection(m_flowDirection->getData(static_cast<size_t>(posY), static_cast<size_t>(posX)), posY, posX);
                    posCheckedNodeList = static_cast<size_t>(posY) * m_flowDirection->getCols() + static_cast<size_t>(posX);

                    //Se extrapolar os limites da matriz, sai do loop
                    if (posX < 0 || posY < 0 || posX > static_cast<int>(m_flowDirection->getCols() - 1) || posY > static_cast<int>(m_flowDirection->getRows() - 1))
                        break;

                    if (m_flowDirection->getData(static_cast<size_t>(posY), static_cast<size_t>(posX)) == m_flowDirection->getNoDataValue()) //Se chegar em um NODATA, sai do loop
                    {
                        m_flowAccumulation->setData(static_cast<size_t>(posY), static_cast<size_t>(posX), m_flowDirection->getNoDataValue());
                        break;
                    }

                    // Acumula as células anteriores
                    m_flowAccumulation->setData(static_cast<size_t>(posY), static_cast<size_t>(posX), m_flowAccumulation->getData(static_cast<size_t>(posY), static_cast<size_t>(posX)) + totalCells + 1);
                }
            }
        }
    }
}

bool FlowAccumulation::neighbourCellsAnalyzed(int yc, int xc)
{
    //na openlist:,         'direções de apontamento para a célula central
    //For y = -1 To 1       2  4  8
    //posY = (Yc + y)       1    x   16
    //For x = -1 To 1       128   64  32
    //posX = (xc + x)

    int xi = 0, yi = 0;
    bool cellContribution = false;

    for (short y = -1; y <= 1; ++y)
    {
        yi = yc + y;
        for (short x = -1; x <= 1; ++x)
        {
            xi = xc + x;

            // Evita a análise da própria célula (nó central)
            if (x == 0 && y == 0)
            {
                continue;
            }

            // Evita sair dos limites do raster
            if (xi < 0 || yi < 0 || static_cast<size_t>(xi) >= m_flowDirection->getCols() || static_cast<size_t>(yi) >= m_flowDirection->getRows())
            {
                continue;
            }

            // Se houver uma das células vizinhas apontando para o centro, aciona o flag
            if (HeuristicSinkRemovalUtil::relativeIncipientFlowDirection(xi, xc, yi, yc) == m_flowDirection->getData(static_cast<size_t>(yi), static_cast<size_t>(xi)))
            {
                cellContribution = true;
            }

            // Se a célula vizinha estiver apontando para o centro, mas não tiver sido checada ainda, retorna falso.
            if (cellContribution == true && m_checkedNodeList[static_cast<size_t>(yi) * m_flowDirection->getCols() + static_cast<size_t>(xi)] == false)
            {
                return false;
            }

            // Reseta para verificar a próxima célula
            cellContribution = false;
        }
    }

    //Se passar por todas as células, e nenhuma contribuir ou todas já tiverem sido checadas então
    return true;
}

}
}
}
