/*
* Developed by UNISINOS
* author: Cristian Gabriel de Abreu Heylmann
* email: cristianheylmann@unisinos.br
* date: October, 2019
*/

#ifndef CONTINENTAL_HYDROTOOLS_DOMAIN_FLOW_ACCUMULATION_COMMAND_INPUT_H
#define CONTINENTAL_HYDROTOOLS_DOMAIN_FLOW_ACCUMULATION_COMMAND_INPUT_H

#include <cstddef>
#include <QString>
#include <QStringList>
#include "AbstractCommandInput.h"
#include "FileCommand.h"


namespace continental {
namespace hydrotools {
namespace domain
{
/// <summary>
/// Classe base da célula, para indicar a posição na matriz
/// </summary>
class FlowAccumulationCommandInput : AbstractCommandInput
{
public:
    /// Construtor
    FlowAccumulationCommandInput(QStringList argv);
    void prepare();
    void setFlowDirectionInput (const FileCommand flowDirectionInput);
    void setFlowAccumulationOutput (const FileCommand flowAccumulationOutput);

    FileCommand getFlowDirectionInput() const;
    FileCommand getFlowAccumulationOutput() const;

private:
    FileCommand m_flowDirectionInput;
    FileCommand m_flowAccumulationOutput;
};



}
}
}

#endif // CONTINENTAL_HYDROTOOLS_DOMAIN_FLOW_ACCUMULATION_COMMAND_INPUT_H
