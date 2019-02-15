#include <continental/datamanagement/RasterFile.h>
#include "continental/hydrotools/StreamDefinition.h"

using namespace continental::datamanagement;
using namespace std;

namespace continental
{
namespace hydrotools
{

size_t StreamDefinition::getThresholdValue() const
{
    return m_threshold;
}

shared_ptr<Raster<short>> StreamDefinition::getStreamDefinition() const
{
    return m_streamDef;
}

shared_ptr<Raster<float>> StreamDefinition::getStreamGroups() const
{
    return m_streamGroups;
}

void StreamDefinition::setFlowAccumulation(shared_ptr<Raster<float>> flowAccumulation, float thresoldValue, ThresholdType thresType)
{
    m_flowAcc = flowAccumulation;
    calculateThreshold(thresoldValue, thresType);
    m_streamDef = make_shared<Raster<short>>(m_flowAcc->getRows(), m_flowAcc->getCols(), m_flowAcc->getXOrigin(), m_flowAcc->getYOrigin(), m_flowAcc->getCellSize(), m_flowAcc->getNoDataValue());
}

shared_ptr<Raster<float>> StreamDefinition::getFlowAccumulation() const
{
    return m_flowAcc;
}

void StreamDefinition::readFlowAccumulation(const QString &fileName, float thresoldValue, ThresholdType thresType)
{
    setFlowAccumulation(make_shared<Raster<float>>(RasterFile<float>::loadRasterByFile(fileName)), thresoldValue, thresType);
}

void StreamDefinition::readStreamGroupsData(const QString &fileName)
{
    setStreamGroups(make_shared<Raster<float>>(RasterFile<float>::loadRasterByFile(fileName)));
}

void StreamDefinition::calculateThreshold(float value, ThresholdType thrType)
{

    if (thrType == ThresholdType::PercentualOfMaximumCells)
    {
        //Multiplico o máximo nº de células acumuladas por um percentual
        m_threshold = static_cast<size_t>(m_flowAcc->getMaxValue() * value);
    }
    else if (thrType == ThresholdType::NumberOfCells)
    {
        // Retorna o próprio valor
        m_threshold = static_cast<size_t>(value);
    }
    else if (thrType == ThresholdType::Area)
    {
        // Considera que 1º tenha aprox. 111km
        m_threshold = static_cast<size_t>(static_cast<double>(value) / (std::pow(m_flowAcc->getCellSize() * 111, 2)));
    }

}

void StreamDefinition::defineStreams()
{

    if (m_threshold < 1)
    {
        throw std::invalid_argument("Minimum number of cells to form a stream is insufficient.");
    }
    if (m_streamGroups == nullptr) //Se o arquivo de grupos não for carregado, atribui um grupo único
    {
        setUniqueTreshold();
    }

    for (size_t nRow = 0; nRow < m_streamDef->getRows(); nRow++)
    {
        for (size_t nCol = 0; nCol < m_streamDef->getCols(); nCol++)
        {
            //Se encontrar NODATA escreve NOData
            if (qFuzzyCompare(m_flowAcc->getData(nRow, nCol), m_flowAcc->getNoDataValue()))
            {
                m_streamDef->setData(nRow, nCol, static_cast<short>(m_flowAcc->getNoDataValue()));
            }
            //Se for maior do que o threshold, marca a célula
            else if (m_flowAcc->getData(nRow, nCol) >= m_streamGroups->getData(nRow, nCol))
            {
                m_streamDef->setData(nRow, nCol, 1);
            }
            //Se for inferior ao threshold, marca a célula como noData
            else
            {
                // _FlowAcc.NoDataValue
                m_streamDef->setData(nRow, nCol, 0);
            }

        }
    }

}

void StreamDefinition::setUniqueTreshold()
{
    //Cria um novo Raster de Grupos, sendo que este será para um grupo de threshold único
    m_streamGroups = make_shared<Raster<float>>(m_flowAcc->getRows(), m_flowAcc->getCols(), m_flowAcc->getXOrigin(), m_flowAcc->getYOrigin(), m_flowAcc->getCellSize(), m_flowAcc->getNoDataValue());

    for (size_t nRow = 0; nRow < m_streamDef->getRows(); nRow++)
    {
        for (size_t nCol = 0; nCol < m_streamDef->getCols(); nCol++)
        {
            m_streamGroups->setData(nRow, nCol, m_threshold);
        }
    }

}

}
}
