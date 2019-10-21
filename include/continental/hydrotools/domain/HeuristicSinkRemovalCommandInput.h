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
/// <summary>
/// Classe base da célula, para indicar a posição na matriz
/// </summary>
class HeuristicSinkRemovalCommandInput : AbstractCommandInput
{
public:
    /// Construtor
    HeuristicSinkRemovalCommandInput(QStringList args);
    void prepare();

    size_t getMaxOpenList() const;
    size_t getMaxClosedList() const;
    float getWeightFunctionG() const;
    HeuristicSinkRemoval::ProcessingMode getProcessingAlgorithm() const;

    FileCommand getDemInput() const;
    FileCommand getSinkDestroyOutput() const;
    FileCommand getFlowDirectionOutput() const;

protected:
    void setMaxOpenList (const size_t maxOpenList);
    void setMaxClosedList (const size_t maxClosedList);
    void setWeightFunctionG (const float weightFunctionG);
    void setProcessingAlgorithm (const HeuristicSinkRemoval::ProcessingMode processingAlgorithm);

    void setDemInput (const FileCommand demInput);
    void setSinkDestroyOutput (const FileCommand sinkDestroyOutput);
    void setFlowDirectionOutput (const FileCommand flowDirectionOutput);


private:
    size_t m_maxOpenList;
    size_t m_maxClosedList;
    float m_weightFunctionG;
    HeuristicSinkRemoval::ProcessingMode m_processingAlgorithm;
	
    FileCommand m_demInput;
    FileCommand m_sinkDestroyOutput;
    FileCommand m_flowDirectionOutput;
	
};



}
}
}

#endif // CONTINENTAL_HYDROTOOLS_DOMAIN_HEURISTIC_SINK_REMOVAL_COMMAND_INPUT_H
