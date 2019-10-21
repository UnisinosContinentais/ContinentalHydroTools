#include "continental/hydrotools/domain/CatchmentCommandInput.h"
#include "continental/hydrotools/exception/CatchmentDelineationIsNotValidInputCommandException.h"
#include <iostream>
#include <exception>

using namespace std;
using namespace continental::hydrotools::domain;
using namespace continental::hydrotools::exception;

namespace continental {
namespace hydrotools {
namespace domain {

CatchmentCommandInput::CatchmentCommandInput(QStringList argv)
{
    m_argv = argv;
}

/*
 * Modelo de entrada do Console
 * ===============================
 * [2] = pathFlowDirectionInput
 * [3] = groupFlowDirectionInput
 * [4] = pathStreamSegmentationInput
 * [5] = groupStreamSegmentationInput
 * [6] = pathCatchmentDelineationOutput
 * [7] = groupCatchmentDelineationOutput
*/
void CatchmentCommandInput::prepare()
{
    try
    {
        if(m_argv.length() != 8)
        {
            throw;
        }

        //Parse dos parametros de entradas do console
        auto fileFlowDirectionInput = FileCommand(m_argv[2], m_argv[3]);
        auto fileStreamSegmentationInput = FileCommand(m_argv[4], m_argv[5]);
        auto fileCatchmentDelineationOutput = FileCommand(m_argv[6], m_argv[7]);

        //prepara o objeto
        setFlowDirectionInput(fileFlowDirectionInput);
        setStreamSegmentationInput(fileStreamSegmentationInput);
        setCatchmentDelineationOutput(fileCatchmentDelineationOutput);
    }
    catch (...)
    {
        throw catchmentDelineationIsNotValidInputCommandException;
    }
}


void CatchmentCommandInput::setFlowDirectionInput(const FileCommand flowDirectionInput)
{
    m_flowDirectionInput = flowDirectionInput;
}

FileCommand CatchmentCommandInput::getFlowDirectionInput()
{
    return m_flowDirectionInput;
}

void CatchmentCommandInput::setStreamSegmentationInput(const FileCommand streamSegmentationInput)
{
    m_streamSegmentationInput = streamSegmentationInput;
}

FileCommand CatchmentCommandInput::getStreamSegmentationInput()
{
    return m_streamSegmentationInput;
}

void CatchmentCommandInput::setCatchmentDelineationOutput(const FileCommand catchmentDelineationOutput)
{
    m_catchmentDelineationOutput = catchmentDelineationOutput;
}

FileCommand CatchmentCommandInput::getCatchmentDelineationOutput()
{
    return m_catchmentDelineationOutput;
}

}
}
}




