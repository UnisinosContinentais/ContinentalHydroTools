#include "continental/hydrotools/service/StreamDefinition.h"
#include "continental/hydrotools/exception/StreamDefinitionIsNotValidInputCommandException.h"
#include "continental/hydrotools/constant/MensageConstant.h"


using namespace continental::datamanagement;
using namespace continental::hydrotools::exception;
using namespace continental::hydrotools::constant;
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

shared_ptr<Raster<double>> StreamDefinition::getStreamGroups() const
{
    return m_streamGroups;
}

void StreamDefinition::setFlowAccumulation(shared_ptr<Raster<int>> flowAccumulation, double thresoldValue, ThresholdType thresoldType)
{
    m_flowAcc = flowAccumulation;
    calculateThreshold(thresoldValue, thresoldType);
    m_streamDef = make_shared<Raster<short>>(m_flowAcc->getRows(), m_flowAcc->getCols(), m_flowAcc->getXOrigin(), m_flowAcc->getYOrigin(), m_flowAcc->getCellSize(), m_flowAcc->getNoDataValue());
}

shared_ptr<Raster<int>> StreamDefinition::getFlowAccumulation() const
{
    return m_flowAcc;
}

StreamDefinition::StreamDefinition()
{

}

void StreamDefinition::calculateThreshold(double value, ThresholdType thresoldType)
{

    if (thresoldType == ThresholdType::PercentualOfMaximumCells)
    {
        //Multiplico o máximo nº de células acumuladas por um percentual
        if(value < 0.1 || value > 100)
        {
            throw continental::hydrotools::exception::StreamDefinitionIsNotValidInputCommandException(MensageConstant::PercentualOfMaximumCellsISInvalidParameter);
        }
        auto aux = (value == 100) ? 0.01 : (100 - value);

        m_threshold = (static_cast<size_t>(m_flowAcc->calculateMaxValue() * aux)) / static_cast<size_t>(m_flowAcc->calculateMaxValue());
    }
    else if (thresoldType == ThresholdType::NumberOfCells)
    {
        // Retorna o próprio valor
        m_threshold = static_cast<size_t>(value);

        if (m_threshold <= 0)
        {
            throw continental::hydrotools::exception::StreamDefinitionIsNotValidInputCommandException(MensageConstant::TheMinimumNumberOfCellsToFormFlowIsInsufficient);
        }
    }
    else if (thresoldType == ThresholdType::Area)
    {
        auto aux = static_cast<double>(value) * pow(10, 6);
        m_threshold = static_cast<size_t>(aux / (std::pow(m_flowAcc->getCellSize(), 2)));
    }

}

void StreamDefinition::defineStreams()
{
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
            if (data == m_flowAcc->getNoDataValue())
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

void StreamDefinition::validParameter()
{
    if (m_threshold < 1)
    {
        throw continental::hydrotools::exception::StreamDefinitionIsNotValidInputCommandException(MensageConstant::TheMinimumNumberOfCellsToFormFlowIsInsufficient);
    }
}

void StreamDefinition::setUniqueTreshold()
{
    //Cria um novo Raster de Grupos, sendo que este será para um grupo de threshold único
    m_streamGroups = make_shared<Raster<double>>(m_flowAcc->getRows(), m_flowAcc->getCols(), m_flowAcc->getXOrigin(), m_flowAcc->getYOrigin(), m_flowAcc->getCellSize(), m_flowAcc->getNoDataValue());

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
