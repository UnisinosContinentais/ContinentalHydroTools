/*
* Developed by UNISINOS
* author: Cristian Gabriel de Abreu Heylmann
* email: cristianheylmann@unisinos.br
* date: October, 2019
*/

#ifndef CONTINENTAL_HYDROTOOLS_DOMAIN_STREAMDEFINITION_COMMAND_INPUT_H
#define CONTINENTAL_HYDROTOOLS_DOMAIN_STREAMDEFINITION_COMMAND_INPUT_H

#include <cstddef>
#include <QString>
#include <QString>
#include "AbstractCommandInput.h"
#include "FileCommand.h"
#include "continental/hydrotools/service/StreamDefinition.h"

using namespace continental::hydrotools::service;

namespace continental {
namespace hydrotools {
namespace domain {
/// <summary>
/// Classe base da célula, para indicar a posição na matriz
/// </summary>
class StreamDefinitionCommandInput : AbstractCommandInput
{
public:
    /// Construtor
    StreamDefinitionCommandInput(QStringList argv);
    void prepare();

    void setThresoldValue (const float thresoldValue);
    void setThresholdType(const StreamDefinition::ThresholdType thresholdType);
	
    void setFlowAccumulationInput (const FileCommand flowAccumulationData);
    void setStreamDefinitionOutput (const FileCommand streamDefinitionOutput);
	
    float getThresoldValue() const;
    StreamDefinition::ThresholdType getThresholdType() const;

    FileCommand getFlowAccumulationInput() const;
    
    FileCommand getStreamDefinitionOutput() const;

private:
    float m_thresoldValue;
    StreamDefinition::ThresholdType m_thresholdType;
    
    FileCommand m_flowAccumulationInput;
    FileCommand m_streamDefinitionOutput;
};


}
}
}

#endif // CONTINENTAL_HYDROTOOLS_DOMAIN_CATCHMENT_COMMAND_INPUT_H
