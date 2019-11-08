#include "continental/hydrotools/domain/FlowAccumulationCommandInput.h"
#include "continental/hydrotools/exception/FlowAccumulationIsNotValidInputCommandException.h"
using namespace std;
using namespace continental::hydrotools::domain;
using namespace continental::hydrotools::exception;

namespace continental {
namespace hydrotools {
namespace domain {

FlowAccumulationCommandInput::FlowAccumulationCommandInput(QStringList argv)
{
    m_argv = argv;
}

/*
 * Modelo de entrada do Console
 * ===============================
 * [2] = pathFlowDirectionInput
 * [3] = groupFlowDirectionInput
 * [4] = pathFlowAccumulationOutput
 * [5] = groupFlowAccumulationOutput
*/
void FlowAccumulationCommandInput::prepare()
{

    if(m_argv.length() != 6)
    {
        throw exception::FlowAccumulationIsNotValidInputCommandException();
    }

    //Parse dos parametros de entradas do console
    auto fileFlowDirectionInput = FileCommand(m_argv[2], m_argv[3]);
    auto fileFlowAccumulationOutput = FileCommand(m_argv[4], m_argv[5]);

    //prepara o objeto
    this->setFlowDirectionInput(fileFlowDirectionInput);
    this->setFlowAccumulationOutput(fileFlowAccumulationOutput);

}

void FlowAccumulationCommandInput::setFlowDirectionInput(const FileCommand flowDirectionInput)
{
    m_flowDirectionInput = flowDirectionInput;
}

FileCommand FlowAccumulationCommandInput::getFlowDirectionInput() const
{
    return m_flowDirectionInput;
}

FileCommand FlowAccumulationCommandInput::getFlowAccumulationOutput() const
{
    return m_flowAccumulationOutput;
}

void FlowAccumulationCommandInput::setFlowAccumulationOutput(const FileCommand flowAccumulationOutput)
{
    m_flowAccumulationOutput = flowAccumulationOutput;
}

}
}
}




