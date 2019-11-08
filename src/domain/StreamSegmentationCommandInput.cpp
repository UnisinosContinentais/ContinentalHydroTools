#include "continental/hydrotools/domain/StreamSegmentationCommandInput.h"
#include "continental/hydrotools/exception/StreamSegmentationIsNotValidInputCommandException.h"
#include <QStringList>

using namespace std;
using namespace continental::hydrotools::domain;
using namespace continental::hydrotools::exception;

namespace continental {
namespace hydrotools {
namespace domain {

StreamSegmentationCommandInput::StreamSegmentationCommandInput(QStringList argv)
{
    m_argv = argv;
}

/*
 * Modelo de entrada do Console
 * ===============================
 * [2] = pathFlowDirectionInput
 * [3] = groupFlowDirectionInput
 * [5] = pathWatershedDelineationOutput
 * [6] = groupWatershedDelineationOutput
 * [7] = col
 * [8] = row
*/
void StreamSegmentationCommandInput::prepare()
{
    if(m_argv.length() != 8)
    {
        throw exception::StreamSegmentationIsNotValidInputCommandException();
    }

    //Parse dos parametros de entradas do console
    auto fileFlowDirectionInput = FileCommand(m_argv[2], m_argv[3]);
    auto fileStreamDefinitionInput = FileCommand(m_argv[4], m_argv[5]);
    auto fileStreamSegmentationOutput = FileCommand(m_argv[6], m_argv[7]);

    //prepara o objeto
    setFlowDirectionInput(fileFlowDirectionInput);
    setStreamDefinitionInput(fileStreamDefinitionInput);
    setStreamSegmentationOutput(fileStreamSegmentationOutput);
}

void StreamSegmentationCommandInput::setStreamDefinitionInput(const FileCommand streamDefinitionInput)
{
    m_streamDefinitionInput = streamDefinitionInput;
}

void StreamSegmentationCommandInput::setFlowDirectionInput(const FileCommand flowDirectionInput)
{
    m_flowDirectionInput = flowDirectionInput;
}

void StreamSegmentationCommandInput::setStreamSegmentationOutput(const FileCommand streamSegmentationOutput)
{
    m_streamSegmentationOutput = streamSegmentationOutput;
}

FileCommand StreamSegmentationCommandInput::getStreamDefinitionInput() const
{
    return m_streamDefinitionInput;
}

FileCommand StreamSegmentationCommandInput::getFlowDirectionInput() const
{
    return m_flowDirectionInput;
}

FileCommand StreamSegmentationCommandInput::getStreamSegmentationOutput() const
{
    return m_streamSegmentationOutput;
}

}
}
}




