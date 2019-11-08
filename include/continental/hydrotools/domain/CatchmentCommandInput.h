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
/** @brief Classe Comandos de Entrada para o Método de delineação de sub-bacias
 *  Esta classe é derivada de AbstractCommandInput
 */
class CatchmentCommandInput : AbstractCommandInput
{
public:
    /// Construtor
    CatchmentCommandInput(QStringList argv);

    /// Função que executa a preparação/trsnformação dos comandos de entrada em parâmetros
    void prepare();

    /// Função para atualizar o valor do arquivo de input do Flow Direction.
    /// @param FileCommand com valor do arquivo e nome do dataset do hdf5
    void setFlowDirectionInput (const FileCommand flowDirectionInput);

    /// Função para atualizar o valor do arquivo de input do Stream Segmentation.
    /// @param FileCommand com valor do arquivo e nome do dataset do hdf5
    void setStreamSegmentationInput (const FileCommand streamSegmentationInput);

    /// Função para atualizar o valor do arquivo de output do Stream Segmentation.
    /// @param FileCommand com valor do arquivo e nome do dataset do hdf5
    void setCatchmentDelineationOutput (const FileCommand catchmentDelineationOutput);

    /// Função Getter do arquivo de entrada do Flow Direction.
    FileCommand getFlowDirectionInput();

    /// Função Getter do arquivo de entrada do Stream Segmentation.
    FileCommand getStreamSegmentationInput();

    /// Função Getter do arquivo de saída do Catchment Delineation.
    FileCommand getCatchmentDelineationOutput();

    /// Destrutor
    ~CatchmentCommandInput() = default;
private:
    /// Atributo
    FileCommand m_flowDirectionInput;

    /// Atributo
    FileCommand m_streamSegmentationInput;

    /// Atributo
    FileCommand m_catchmentDelineationOutput;
};

}
}
}

#endif // CONTINENTAL_HYDROTOOLS_DOMAIN_CATCHMENT_COMMAND_INPUT_H
