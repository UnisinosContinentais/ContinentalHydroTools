/*
* Developed by UNISINOS
* author: Cristian Gabriel de Abreu Heylmann
* email: cristianheylmann@unisinos.br
* date: October, 2019
*/

#ifndef CONTINENTAL_HYDROTOOLS_DOMAIN_CATCHMENT_COMMAND_INPUT_H
#define CONTINENTAL_HYDROTOOLS_DOMAIN_CATCHMENT_COMMAND_INPUT_H


#include <cstddef>
#include <QString>
#include <QString>
#include "AbstractCommandInput.h"
#include "FileCommand.h"

namespace continental {
namespace hydrotools {
namespace domain {

class CatchmentCommandInput : AbstractCommandInput
{
public:
    /// Construtor
    CatchmentCommandInput(QStringList argv);
    void prepare();

    void setFlowDirectionInput (const FileCommand flowDirectionInput);
    void setStreamSegmentationInput (const FileCommand streamSegmentationInput);
    void setCatchmentDelineationOutput (const FileCommand catchmentDelineationOutput);
    FileCommand getFlowDirectionInput();
    FileCommand getStreamSegmentationInput();
    FileCommand getCatchmentDelineationOutput();

private:
    FileCommand m_flowDirectionInput;
    FileCommand m_streamSegmentationInput;
    FileCommand m_catchmentDelineationOutput;
};

}
}
}

#endif // CONTINENTAL_HYDROTOOLS_DOMAIN_CATCHMENT_COMMAND_INPUT_H
