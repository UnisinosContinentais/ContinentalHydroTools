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
/** @brief Classe Comandos de Entrada para o Método de Segmentação do Rio
 *  Esta classe é derivada de AbstractCommandInput
 */
class StreamSegmentationCommandInput : AbstractCommandInput
{
public:
    /// Construtor
    StreamSegmentationCommandInput(QStringList argv);

    /// Função que executa a preparação/trsnformação dos comandos de entrada em parâmetros
    void prepare();

    /// Função Getter do arquivo de entrada do Stream Definition.
    FileCommand getStreamDefinitionInput() const;

    /// Função Getter do arquivo de entrada do Flow Direction.
    FileCommand getFlowDirectionInput() const;

    /// Função Getter do arquivo de saída do Stream Segmentation.
    FileCommand getStreamSegmentationOutput() const;

    /// Destrutor
    ~StreamSegmentationCommandInput() = default;

protected:
    /// Função para atualizar o valor do arquivo de entrada do Stream Definition.
    /// @param FileCommand com valor do arquivo e nome do dataset do hdf5
    void setStreamDefinitionInput (const FileCommand streamDefinitionInput);

    /// Função para atualizar o valor do arquivo de entrada do Flow Direction.
    /// @param FileCommand com valor do arquivo e nome do dataset do hdf5
    void setFlowDirectionInput (const FileCommand flowDirectionInput);

    /// Função para atualizar o valor do arquivo de saída do Stream Segmentation.
    /// @param FileCommand com valor do arquivo e nome do dataset do hdf5
    void setStreamSegmentationOutput (const FileCommand streamSegmentationOutput);

private:
    /// Atributo
    FileCommand m_streamDefinitionInput;

    /// Atributo
    FileCommand m_flowDirectionInput;

    /// Atributo
    FileCommand m_streamSegmentationOutput;
};



}
}
}

#endif // CONTINENTAL_HYDROTOOLS_DOMAIN_STREAMSEGMENTATION_COMMAND_INPUT_H
