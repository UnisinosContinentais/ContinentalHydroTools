/*
* Developed by Projeto Continentais and Petrobras
* author: Lucas G. F. Alves
* email: gabrielfa@unisinos.br
* date: January, 2019
*/
#ifndef CONTINENTAL_HYDROTOOLS_CONSTANT_MESAGECONSTANT_H
#define CONTINENTAL_HYDROTOOLS_CONSTANT_MESAGECONSTANT_H

#include <QString>

namespace continental {
namespace hydrotools {
namespace constant {

/** @brief Classe de constantes.
 * Esta classe contém variáveis constantes usadas pelo projeto.
 */
namespace MensageConstant {
    //Global
    static const std::string InvalidArguments = "InvalidArguments";
    static const std::string InvalidArgumentsPattern = "InvalidArgumentsPattern";
    static const std::string TheMinimumNumberOfCellsToFormFlowIsInsufficient = "TheMinimumNumberOfCellsToFormFlowIsInsufficient";
    static const std::string PercentualOfMaximumCellsISInvalidParameter = "PercentualOfMaximumCellsISInvalidParameter";
    static const std::string FileNotFound = "FileNotFound";


    //SinkDestroy
    static const std::string InvalidArgumentsSinkDestroy = "InvalidArgumentsSinkDestroy";
    static const std::string SinkDestroyProcessException = "SinkDestroyProcessException";

    //FlowAccumulation
    static const std::string InvalidArgumentsFlowAccumulation = "InvalidArgumentsFlowAccumulation";
    static const std::string FlowAccumulationProcessException = "FlowAccumulationProcessException";

    //StreamDefinition
    static const std::string InvalidArgumentsStreamDefinition = "InvalidArgumentsStreamDefinition";
    static const std::string StreamDefinitionProcessException = "StreamDefinitionProcessException";

    //StreamSegmentation
    static const std::string InvalidArgumentsStreamSegmentation = "InvalidArgumentsStreamSegmentation";
    static const std::string StreamSegmentationProcessException = "StreamSegmentationProcessException";

    //WatershedDelineation
    static const std::string InvalidArgumentsWatershedDelineation = "InvalidArgumentsWatershedDelineation";
    static const std::string WatershedDelineationProcessException = "WatershedDelineationProcessException";

    //CatchmentDelineation
    static const std::string InvalidArgumentsCatchmentDelineation = "InvalidArgumentsCatchmentDelineation";
    static const std::string CatchmentDelineationProcessException = "CatchmentDelineationProcessException";
}


}
}
}

#endif // CONTINENTAL_HYDROTOOLS_CONSTANT_MESAGECONSTANT_H
