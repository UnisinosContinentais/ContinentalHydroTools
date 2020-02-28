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
/** @brief Classe Comandos de Entrada para o Método de Acumulação do Fluxo
 *  Esta classe é derivada de AbstractCommandInput
 */
class FlowAccumulationCommandInput : AbstractCommandInput
{
public:
    /// Construtor
    FlowAccumulationCommandInput(QStringList argv);

    /// Função que executa a preparação/trsnformação dos comandos de entrada em parâmetros
    void prepare();

    /// Função para atualizar o valor do arquivo de input do Flow Direction.
    /// @param FileCommand com valor do arquivo e nome do dataset do hdf5
    void setFlowDirectionInput (const FileCommand flowDirectionInput);

    /// Função para atualizar o valor do arquivo de input do Flow Accumulation.
    /// @param FileCommand com valor do arquivo e nome do dataset do hdf5
    void setFlowAccumulationOutput (const FileCommand flowAccumulationOutput);

    /// Função Getter do arquivo de entrada do Flow Direction.
    FileCommand getFlowDirectionInput() const;

    /// Função Getter do arquivo de saída do Flow Accumulation.
    FileCommand getFlowAccumulationOutput() const;

    /// Destrutor
    ~FlowAccumulationCommandInput() = default;
private:
    /// Atributo
    FileCommand m_flowDirectionInput;

    /// Atributo
    FileCommand m_flowAccumulationOutput;
};



}
}
}

#endif // CONTINENTAL_HYDROTOOLS_DOMAIN_FLOW_ACCUMULATION_COMMAND_INPUT_H
