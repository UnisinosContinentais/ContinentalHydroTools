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

namespace continental {
namespace hydrotools {
namespace domain {
/** @brief Classe Comandos de Entrada para o Método de Definição do Rio
 *  Esta classe é derivada de AbstractCommandInput
 */
class StreamDefinitionCommandInput : AbstractCommandInput
{
public:
    /// Construtor
    StreamDefinitionCommandInput(QStringList argv);

    /// Função que executa a preparação/trsnformação dos comandos de entrada em parâmetros
    void prepare();

    void setThresoldValue (const float thresoldValue);
    void setThresholdType(const service::StreamDefinition::ThresholdType thresholdType);

    /// Função Getter do atributo ThresoldValue.
    float getThresoldValue() const;

    /// Função Getter do atributo ThresholdType
    service::StreamDefinition::ThresholdType getThresholdType() const;
	
    void setFlowAccumulationInput (const FileCommand flowAccumulationData);
    void setStreamDefinitionOutput (const FileCommand streamDefinitionOutput);
	
    /// Função Getter do arquivo de entrada do Flow Accumulation.
    FileCommand getFlowAccumulationInput() const;
    
    /// Função Getter do arquivo de entrada do Stream Definition.
    FileCommand getStreamDefinitionOutput() const;

    /// Destrutor
    ~StreamDefinitionCommandInput() = default;

private:
    /// Atributo
    float m_thresoldValue;

    /// Atributo
    service::StreamDefinition::ThresholdType m_thresholdType;
    
    /// Atributo
    FileCommand m_flowAccumulationInput;

    /// Atributo
    FileCommand m_streamDefinitionOutput;
};


}
}
}

#endif // CONTINENTAL_HYDROTOOLS_DOMAIN_CATCHMENT_COMMAND_INPUT_H
