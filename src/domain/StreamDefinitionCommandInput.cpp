#include "continental/hydrotools/domain/StreamDefinitionCommandInput.h"
#include "continental/hydrotools/service/StreamDefinition.h"
#include "continental/hydrotools/exception/StreamDefinitionIsNotValidInputCommandException.h"
#include <QStringList>

using namespace std;
using namespace continental::hydrotools::domain;
using namespace continental::hydrotools::exception;
using namespace continental::datamanagement;
using namespace std;

namespace continental {
namespace hydrotools {
namespace domain {

StreamDefinitionCommandInput::StreamDefinitionCommandInput(QStringList argv)
{
  m_argv = argv;
}

///
/// Modelo de entrada do Console
/// Parametros de Entrda Vetor de 7 posições
/// [2] = pathFlowAccumulationInput
/// [3] = groupFlowAccumulationInput
/// [4] = pathStreamDefinitionOutput
/// [5] = grouStreamDefinitionOutput
/// [6] = thresoldValue
///
void StreamDefinitionCommandInput::prepare()
{
    if(m_argv.length() != 8)
    {
        throw exception::StreamDefinitionIsNotValidInputCommandException();
    }

    //Parse dos parametros de entradas do console
    auto fileFlowAccumulationInput = FileCommand(m_argv[2], m_argv[3]);
    auto fileStreamDefinitionOutput = FileCommand(m_argv[4], m_argv[5]);

    StreamDefinition::ThresholdType thresholdType = static_cast<StreamDefinition::ThresholdType>(m_argv[6].toInt());
    size_t thresoldValue = static_cast<size_t>(m_argv[7].toInt());

    //prepara o objeto
    this->setFlowAccumulationInput(fileFlowAccumulationInput);
    this->setThresoldValue(thresoldValue);
    this->setThresholdType(thresholdType);
    this->setStreamDefinitionOutput(fileStreamDefinitionOutput);
}

void StreamDefinitionCommandInput::setThresoldValue(const float thresoldValue)
{
    m_thresoldValue = thresoldValue;
}

void StreamDefinitionCommandInput::setFlowAccumulationInput(const FileCommand flowAccumulationData)
{
    m_flowAccumulationInput = flowAccumulationData;
}

void StreamDefinitionCommandInput::setStreamDefinitionOutput(const FileCommand streamDefinitionOutput)
{
    m_streamDefinitionOutput = streamDefinitionOutput;
}

void StreamDefinitionCommandInput::setThresholdType(const StreamDefinition::ThresholdType thresholdType)
{
    m_thresholdType = thresholdType;
}

float StreamDefinitionCommandInput::getThresoldValue() const
{
    return m_thresoldValue;
}

StreamDefinition::ThresholdType StreamDefinitionCommandInput::getThresholdType() const
{
    return m_thresholdType;
}

FileCommand StreamDefinitionCommandInput::getFlowAccumulationInput() const
{
    return m_flowAccumulationInput;
}

FileCommand StreamDefinitionCommandInput::getStreamDefinitionOutput() const
{
    return m_streamDefinitionOutput;
}

}
}
}




