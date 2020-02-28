/*
* Developed by UNISINOS
* author: Cristian Gabriel de Abreu Heylmann
* email: cristianheylmann@unisinos.br
* date: October, 2019
*/

#ifndef CONTINENTAL_HYDROTOOLS_DOMAIN_WATERSHEDDELINEATION_COMMAND_INPUT_H
#define CONTINENTAL_HYDROTOOLS_DOMAIN_WATERSHEDDELINEATION_COMMAND_INPUT_H

#include <cstddef>
#include <QString>
#include <QString>
#include "AbstractCommandInput.h"
#include "FileCommand.h"

namespace continental {
namespace hydrotools {
namespace domain {

/** @brief Classe Comandos de Entrada para o Método de delineação da bacia Hidrográfica
 *  Esta classe é derivada de AbstractCommandInput
 */
class WatershedDelineationCommandInput : AbstractCommandInput
{
public:
    /// Construtor
    WatershedDelineationCommandInput(QStringList argv);

    /// Função que executa a preparação/trsnformação dos comandos de entrada em parâmetros
    void prepare();

    /// Função para atualizar o valor do arquivo de input do Flow Direction.
    /// @param FileCommand com valor do arquivo e nome do dataset do hdf5
    void setFlowDirectionInput (const FileCommand flowDirectionInput);

    /// Função para atualizar o valor do arquivo de output do Watershed Delineation.
    /// @param FileCommand com valor do arquivo e nome do dataset do hdf5
    void setWatershedDelineationOutput (const FileCommand watershedDelineationOutput);

    /// Função Getter do arquivo de entrada do Flow Direction.
    FileCommand getFlowDirectionInput() const;

    /// Função Getter do arquivo de saída do Watershed Delineation.
    FileCommand getWatershedDelineationOutput() const;

    /// Função Getter da posição da Linha
    size_t getRow() const;

    /// Função para atualizar o valor da linha
    /// @param valor da linha
    void setRow(const size_t &row);

    /// Função Getter da posição da Coluna
    size_t getCol() const;

    /// Função para atualizar o valor da coluna
    /// @param valor da coluna
    void setCol(const size_t &col);

    /// Destrutor
    ~WatershedDelineationCommandInput() = default;

private:
    /// Atributo
    FileCommand m_flowDirectionInput;

    /// Atributo
    FileCommand m_watershedDelineationOutput;

    /// Atributo
    size_t m_row;

    /// Atributo
    size_t m_col;
};



}
}
}

#endif // CONTINENTAL_HYDROTOOLS_DOMAIN_WATERSHEDDELINEATION_COMMAND_INPUT_H
