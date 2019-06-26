#include <memory>
#include "continental/hydrotools/HeuristicSinkRemovalUtil.h"
#include "continental/hydrotools/StreamSegmentation.h"
#include "continental/hydrotools/FlowDirection.h"

using namespace std;
using namespace continental::datamanagement;

namespace continental
{
namespace hydrotools
{

StreamSegmentation::StreamSegmentation()
{

}

void StreamSegmentation::setFlowDirection(shared_ptr<Raster<short>> flowDirection)
{
    m_flowDirection = flowDirection;
    m_strSeg = make_shared<Raster<short>>(m_flowDirection->getRows(), m_flowDirection->getCols(), m_flowDirection->getXOrigin(), m_flowDirection->getYOrigin(), m_flowDirection->getCellSize(), m_flowDirection->getNoDataValue());
}

void StreamSegmentation::setStreamDefinition(std::shared_ptr<datamanagement::Raster<short> > streamDefinition)
{
    m_strDef = streamDefinition;
}

void StreamSegmentation::segmentStreams()
{
    short segmentNumber = 1;

    int rows = static_cast<int>(m_strSeg->getRows());
    int cols = static_cast<int>(m_strSeg->getCols());

    for (int row = 0; row < rows; ++row)
    {
        for (int col = 0; col < cols; ++col)
        {
            int i = row;
            int j = col;

            //Atribui valores de NODATA
            if (m_strDef->getData(row, col) == m_strDef->getNoDataValue())
            {
                m_strSeg->setData(row, col, static_cast<short>(m_strDef->getNoDataValue()));
            }
            //Caso estiver sobre a rede de drenagem
            else if (m_strDef->getData(row, col) == 1)
            {
                //Caso a célula estiver sobre um trecho ainda não segmentado
                if (m_strSeg->getData(row, col) == 0)
                {
                    //Identifica a linha e a coluna iniciais
                    int xStart = j;
                    int yStart = i;
                    while (true)
                    {
                        int xEnd = j;
                        int yEnd = i;
                        //Move em direção à próxima célula
                        HeuristicSinkRemovalUtil::moveToFlowDirection(m_flowDirection->getData(i, j), i, j);

                        //Se alcançar os limites da matriz, utiliza o atributo da última célula
                        if (i > rows - 1 || j > cols - 1)
                        {
                            if (m_strSeg->getData(yEnd, xEnd) == 0)
                            {
                                m_strSeg->setData(yEnd, xEnd, segmentNumber);
								++segmentNumber;
                            }
                            updateSegments(yStart, xStart, yEnd, xEnd, m_strSeg->getData(yEnd, xEnd));
                            break;
                        }

                        if (m_strDef->getData(i, j) == m_flowDirection->getNoDataValue())
                        {
                            if (m_strSeg->getData(yEnd, xEnd) == 0)
                            {
                                m_strSeg->setData(yEnd, xEnd, segmentNumber);
								++segmentNumber;
                            }
                            updateSegments(yStart, xStart, yEnd, xEnd, m_strSeg->getData(yEnd, xEnd));
                            //Passa se mover para um NODATA
                            break;
                        }

                        if (verifyStreamOutlet(i, j))
                        {
                            updateSegments(yStart, xStart, yEnd, xEnd, segmentNumber);
                            //Aumenta o número dos segmentos identificados
							++segmentNumber;
                            break;
                        }

                        //Se chegar em uma parte do trecho com atributo definido, atualiza o restante do trecho atual
                        if (m_strSeg->getData(i, j) > 0)
                        {
                            updateSegments(yStart, xStart, yEnd, xEnd, m_strSeg->getData(i, j));
                            break;
                        }
                    }
                }
            }
        }
    }
}

std::shared_ptr<datamanagement::Raster<short>> StreamSegmentation::getStreamSegmentation()
{
    return m_strSeg;
}

bool StreamSegmentation::verifyStreamOutlet(int yc, int xc)
{
    //direções de apontamento para a célula central
    //For y = -1 To 1       2  4  8
    //posY = (Yc + y)       1    x   16
    //For x = -1 To 1       128   64  32
    //posX = (xc + x)

    int xi = 0;
    int yi = 0;
    short nPointingCells = 0;

    for (int y = -1; y <= 1; ++y)
    {
        yi = static_cast<int>(yc) + y;
        for (int x = -1; x <= 1; ++x)
        {
            xi = static_cast<int>(xc) + x;

            //Evita a análise da própria célula (nó central)
            if (x != 0 || y != 0)
            {
                //Evita sair dos limites do raster
                if (xi >= 0 && yi >= 0 && xi < static_cast<int>(m_flowDirection->getCols()) && yi < static_cast<int>(m_flowDirection->getRows()))
                {
                    //Somente se a célula faz parte da rede de drenagem principal
                    auto data = m_strDef->getData(static_cast<size_t>(yi), static_cast<size_t>(xi));
                    //Se a célula analisada apontar para a célula de origem, é contabilizada
                    if (data == 1 && HeuristicSinkRemovalUtil::relativeIncipientFlowDirection(static_cast<size_t>(xi), static_cast<size_t>(xc), static_cast<size_t>(yi), static_cast<size_t>(yc)) == m_flowDirection->getData(yi, xi))
                    {
                        nPointingCells += 1;
                    }
                }

            }
        }
    }

    //Se a mais de uma célula apontar para a célula central, significa que encontrou uma confluência
    return nPointingCells > 1;
}

void StreamSegmentation::updateSegments(int row, int column, int rowLast, int columnLast, short attribute)
{
    //0 = célula ainda não identificada
    int lin = static_cast<int>(row);
    int col = static_cast<int>(column);

    m_strSeg->setData(rowLast, columnLast, attribute);

    while (!(lin == static_cast<int>(rowLast) && col == static_cast<int>(columnLast)))
    {
        m_strSeg->setData(lin, col, attribute);

        HeuristicSinkRemovalUtil::moveToFlowDirection(m_flowDirection->getData(lin, col), lin, col);
        //Cheguei na borda do MDE
        if (col < 0 || lin < 0 || col > (static_cast<int>(m_flowDirection->getCols()) - 1) || lin > (static_cast<int>(m_flowDirection->getRows()) - 1))
        {
            break;
        }
    }

}
}
}
