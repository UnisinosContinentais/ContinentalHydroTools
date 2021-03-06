﻿#include "continental/hydrotools/service/StreamDefinition.h"

using namespace continental::datamanagement;
using namespace std;

namespace continental
{
namespace hydrotools
{
namespace service
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

void StreamDefinition::setFlowAccumulation(shared_ptr<Raster<float>> flowAccumulation, float thresoldValue, ThresholdType thresoldType)
{
    m_flowAcc = flowAccumulation;
    calculateThreshold(thresoldValue, thresoldType);
    m_streamDef = make_shared<Raster<short>>(m_flowAcc->getRows(), m_flowAcc->getCols(), m_flowAcc->getXOrigin(), m_flowAcc->getYOrigin(), m_flowAcc->getCellSize(), m_flowAcc->getNoDataValue());
}

shared_ptr<Raster<float>> StreamDefinition::getFlowAccumulation() const
{
    return m_flowAcc;
}

StreamDefinition::StreamDefinition()
{

}

void StreamDefinition::calculateThreshold(float value, ThresholdType thresoldType)
{

    if (thresoldType == ThresholdType::PercentualOfMaximumCells)
    {
        //Multiplico o máximo nº de células acumuladas por um percentual
        m_threshold = static_cast<size_t>(m_flowAcc->calculateMaxValue() * value);
    }
    else if (thresoldType == ThresholdType::NumberOfCells)
    {
        // Retorna o próprio valor
        m_threshold = static_cast<size_t>(value);
    }
    else if (thresoldType == ThresholdType::Area)
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

    for (size_t row = 0; row < m_streamDef->getRows(); ++row)
    {
        for (size_t col = 0; col < m_streamDef->getCols(); ++col)
        {
            //Se encontrar NODATA escreve NOData
            auto data = m_flowAcc->getData(row, col);
            if (qFuzzyCompare(data, m_flowAcc->getNoDataValue()))
            {
                m_streamDef->setData(row, col, static_cast<short>(m_flowAcc->getNoDataValue()));
            }
            //Se for maior do que o threshold, marca a célula
            else if (data >= m_streamGroups->getData(row, col))
            {
                m_streamDef->setData(row, col, 1);
            }
            //Se for inferior ao threshold, marca a célula como noData
            else
            {
                // _FlowAcc.NoDataValue
                m_streamDef->setData(row, col, 0);
            }
        }
    }

}

void StreamDefinition::setUniqueTreshold()
{
    //Cria um novo Raster de Grupos, sendo que este será para um grupo de threshold único
    m_streamGroups = make_shared<Raster<float>>(m_flowAcc->getRows(), m_flowAcc->getCols(), m_flowAcc->getXOrigin(), m_flowAcc->getYOrigin(), m_flowAcc->getCellSize(), m_flowAcc->getNoDataValue());

    for (size_t row = 0; row < m_streamDef->getRows(); ++row)
    {
        for (size_t col = 0; col < m_streamDef->getCols(); ++col)
        {
            m_streamGroups->setData(row, col, m_threshold);
        }
    }

}

}
}
}
