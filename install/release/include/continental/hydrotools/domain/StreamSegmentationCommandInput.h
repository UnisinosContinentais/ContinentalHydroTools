/*
* Developed by UNISINOS
* author: Cristian Gabriel de Abreu Heylmann
* email: cristianheylmann@unisinos.br
* date: October, 2019
*/

#ifndef CONTINENTAL_HYDROTOOLS_DOMAIN_STREAMSEGMENTATION_COMMAND_INPUT_H
#define CONTINENTAL_HYDROTOOLS_DOMAIN_STREAMSEGMENTATION_COMMAND_INPUT_H

#include <cstddef>
#include <QString>
#include <QString>
#include "AbstractCommandInput.h"
#include "FileCommand.h"


//*******************************************************************
//CLASSES ESPECÍFICAS DE CÉLULAS UTILIZADAS PELO PROGRAMA
//*******************************************************************

namespace continental
{
namespace hydrotools
{
namespace domain
{
/// <summary>
/// Classe base da célula, para indicar a posição na matriz
/// </summary>
class StreamSegmentationCommandInput : AbstractCommandInput
{
public:
    /// Construtor
    StreamSegmentationCommandInput(QStringList argv);
    void prepare();

    FileCommand getStreamDefinitionInput() const;
    FileCommand getFlowDirectionInput() const;
    FileCommand getStreamSegmentationOutput() const;

protected:
    void setStreamDefinitionInput (const FileCommand streamDefinitionInput);
    void setFlowDirectionInput (const FileCommand flowDirectionInput);
    void setStreamSegmentationOutput (const FileCommand streamSegmentationOutput);

private:
    FileCommand m_streamDefinitionInput;
    FileCommand m_flowDirectionInput;
    FileCommand m_streamSegmentationOutput;
};



}
}
}

#endif // CONTINENTAL_HYDROTOOLS_DOMAIN_STREAMSEGMENTATION_COMMAND_INPUT_H
