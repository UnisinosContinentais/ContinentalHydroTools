/*
* Developed by UNISINOS
* author: Cristian Gabriel de Abreu Heylmann
* email: cristianheylmann@unisinos.br
* date: October, 2019
*/

#ifndef CONTINENTAL_HYDROTOOLS_DOMAIN_HEURISTIC_SINK_REMOVAL_COMMAND_INPUT_H
#define CONTINENTAL_HYDROTOOLS_DOMAIN_HEURISTIC_SINK_REMOVAL_COMMAND_INPUT_H

#include <cstddef>
#include <QString>
#include <QString>
#include "AbstractCommandInput.h"
#include "continental/hydrotools/service/HeuristicSinkRemoval.h"
#include "continental/hydrotools/service/HeuristicSinkRemovalUtil.h"
#include "FileCommand.h"

using namespace continental::hydrotools::service;
namespace continental {
namespace hydrotools {
namespace domain {
/** @brief Classe Comandos de Entrada para o Método de Remoção de Descontinuidade
 *  Esta classe é derivada de AbstractCommandInput
 */
class HeuristicSinkRemovalCommandInput : AbstractCommandInput
{
public:
    /// Construtor
    HeuristicSinkRemovalCommandInput(QStringList args);

    /// Função que executa a preparação/trsnformação dos comandos de entrada em parâmetros
    void prepare();

    /// Função Getter do atributo MaxOpenList.
    size_t getMaxOpenList() const;

    /// Função Getter do atributo MaxClosedList.
    size_t getMaxClosedList() const;

    /// Função Getter do atributo WeightFunctionG.
    float getWeightFunctionG() const;

    /// Função Getter do atributo tipo de algoritmo de processamento
    HeuristicSinkRemoval<float>::ProcessingMode getProcessingAlgorithm() const;

    /// Função Getter do arquivo de entrada da Superfície inicial.
    FileCommand getDemInput() const;

    /// Função Getter do arquivo de saída do SinkDestroy.
    FileCommand getSinkDestroyOutput() const;

    /// Função para atualizar o valor do arquivo de saída do Flow Direction.
    /// @param FileCommand com valor do arquivo e nome do dataset do hdf5
    FileCommand getFlowDirectionOutput() const;

    /// Destrutor
    ~HeuristicSinkRemovalCommandInput() = default;
protected:
    /// Função para atualizar o valor do atributo maxOpenList
    /// @param maxOpenList com valor do tipo size_t
    void setMaxOpenList (const size_t maxOpenList);

    /// Função para atualizar o valor do atributo maxClosedList
    /// @param maxClosedList com valor do tipo size_t
    void setMaxClosedList (const size_t maxClosedList);

    /// Função para atualizar o valor do atributo weightFunctionG
    /// @param weightFunctionG com valor do tipo size_t
    void setWeightFunctionG (const float weightFunctionG);

    /// Função para atualizar o valor do atributo weightFunctionG
    /// @param weightFunctionG com valor do tipo size_t
    void setProcessingAlgorithm (const HeuristicSinkRemoval<float>::ProcessingMode processingAlgorithm);

    /// Função para atualizar o valor do arquivo de entrada da Superfície inicial.
    /// @param FileCommand com valor do arquivo e nome do dataset do hdf5
    void setDemInput (const FileCommand demInput);

    /// Função para atualizar o valor do arquivo de saída do Sink and Destroy.
    /// @param FileCommand com valor do arquivo e nome do dataset do hdf5
    void setSinkDestroyOutput (const FileCommand sinkDestroyOutput);

    /// Função para atualizar o valor do arquivo de saída do Flow Direction.
    /// @param FileCommand com valor do arquivo e nome do dataset do hdf5
    void setFlowDirectionOutput (const FileCommand flowDirectionOutput);

private:
    /// Atributo
    size_t m_maxOpenList;

    /// Atributo
    size_t m_maxClosedList;

    /// Atributo
    float m_weightFunctionG;

    /// Atributo
    HeuristicSinkRemoval<float>::ProcessingMode m_processingAlgorithm;

    /// Atributo
    FileCommand m_demInput;

    /// Atributo
    FileCommand m_sinkDestroyOutput;

    /// Atributo
    FileCommand m_flowDirectionOutput;
};



}
}
}

#endif // CONTINENTAL_HYDROTOOLS_DOMAIN_HEURISTIC_SINK_REMOVAL_COMMAND_INPUT_H
