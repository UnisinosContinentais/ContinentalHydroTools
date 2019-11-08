#include "continental/hydrotools/domain/HeuristicSinkRemovalCommandInput.h"
#include "continental/hydrotools/exception/SinkDestroyIsNotValidInputCommandException.h"
#include <QStringList>

using namespace std;
using namespace continental::hydrotools::domain;
using namespace continental::hydrotools::exception;

namespace continental {
namespace hydrotools {
namespace domain {

HeuristicSinkRemovalCommandInput::HeuristicSinkRemovalCommandInput(QStringList args)
{
    m_argv = args;
}
/*
 * Modelo de entrada do Console
 * ===============================
 * [2] = pathDemInput
 * [3] = groupDemInput
 * [4] = pathFlowDirectionOutput
 * [5] = groupFlowDirectionOutput
 * [6] = pathDestroyOutput
 * [7] = groupDestroyOutput
 * [8] = maxOpenList
 * [9] = maxClosedList
 * [10] = weightFunctionG
 * [11] = processingAlgorithm
*/
void HeuristicSinkRemovalCommandInput::prepare()
{

    if(m_argv.length() != 12)
    {
        throw exception::SinkDestroyIsNotValidInputCommandException();
    }

    //Parse dos parametros de entradas do console
    size_t maxOpenList = static_cast<size_t>(m_argv[8].toInt());
    size_t maxClosedList = static_cast<size_t>(m_argv[9].toInt());
    float weightFunctionG = (m_argv[10].toFloat());

    auto fileDemInitialSurfaceInput = FileCommand(m_argv[2], m_argv[3]);
    auto fileFlowDirectionOutput = FileCommand(m_argv[4], m_argv[5]);
    auto fileSinkDestroyOutput = FileCommand(m_argv[6], m_argv[7]);
    auto processingAlgorithm = static_cast<HeuristicSinkRemoval<short>::ProcessingMode>(m_argv[11].toInt());

    if(processingAlgorithm != HeuristicSinkRemoval<short>::ProcessingMode::MHS)
    {
        throw exception::SinkDestroyIsNotValidInputCommandException();
    }

    //prepara o objeto
    setDemInput(fileDemInitialSurfaceInput);
    setMaxOpenList(maxOpenList);
    setMaxClosedList(maxClosedList);
    setWeightFunctionG(weightFunctionG);
    setProcessingAlgorithm(processingAlgorithm);

    this->setFlowDirectionOutput(fileFlowDirectionOutput);
    this->setSinkDestroyOutput(fileSinkDestroyOutput);
}

void HeuristicSinkRemovalCommandInput::setMaxOpenList(const size_t maxOpenList)
{
    m_maxOpenList = maxOpenList;
}

void HeuristicSinkRemovalCommandInput::setMaxClosedList(const size_t maxClosedList)
{
    m_maxClosedList = maxClosedList;
}

void HeuristicSinkRemovalCommandInput::setWeightFunctionG(const float weightFunctionG)
{
    m_weightFunctionG = weightFunctionG;
}

void HeuristicSinkRemovalCommandInput::setProcessingAlgorithm(const HeuristicSinkRemoval<short>::ProcessingMode processingAlgorithm)
{
    m_processingAlgorithm = processingAlgorithm;
}

void HeuristicSinkRemovalCommandInput::setDemInput(const FileCommand demInput)
{
    m_demInput = demInput;
}

void HeuristicSinkRemovalCommandInput::setSinkDestroyOutput(const FileCommand sinkDestroyOutput)
{
    m_sinkDestroyOutput = sinkDestroyOutput;
}

void HeuristicSinkRemovalCommandInput::setFlowDirectionOutput(const FileCommand flowDirectionOutput)
{
    m_flowDirectionOutput = flowDirectionOutput;
}

size_t HeuristicSinkRemovalCommandInput::getMaxOpenList() const
{
    return  m_maxOpenList;
}

size_t HeuristicSinkRemovalCommandInput::getMaxClosedList() const
{
    return  m_maxClosedList;
}

float HeuristicSinkRemovalCommandInput::getWeightFunctionG() const
{
    return  m_weightFunctionG;
}

HeuristicSinkRemoval<short>::ProcessingMode HeuristicSinkRemovalCommandInput::getProcessingAlgorithm() const
{
    return m_processingAlgorithm;
}

FileCommand HeuristicSinkRemovalCommandInput::getDemInput() const
{
    return m_demInput;
}

FileCommand HeuristicSinkRemovalCommandInput::getSinkDestroyOutput() const
{
    return m_sinkDestroyOutput;
}

FileCommand HeuristicSinkRemovalCommandInput::getFlowDirectionOutput() const
{
    return m_flowDirectionOutput;
}

}
}
}




