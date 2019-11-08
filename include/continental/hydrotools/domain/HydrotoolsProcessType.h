/*
* Developed by UNISINOS - Projeto Continentais and Petrobras
* author: Cristian Gabriel de Abreu Heylmann
* email: lbertoldio@unisinos.br
* date: October, 2019
*/
#ifndef CONTINENTALPLUGIN_DOMAIN_FLUVIAL_SYSTEM_HEURISTICSINKREMOVALPROCESSINGMODE_H
#define CONTINENTALPLUGIN_DOMAIN_FLUVIAL_SYSTEM_HEURISTICSINKREMOVALPROCESSINGMODE_H

namespace continental {
namespace hydrotools {
namespace domain {
/** @brief Estrutura de Dados com Tipos de Processos do Continental Hydrotools
 */
typedef enum
{
    ProgramTestMode = 0,
    SinkDestroy = 1,
    FlowAccumulation = 2,
    StreamDefinition = 3,
    StreamSegmention = 4,
    CatchmentDelineation = 5,
    WatershedDelineation = 6
} HydrotoolsProcessType;

} //domain
} // namespace hydrotools
} //continental

#endif // CONTINENTALPLUGIN_DOMAIN_FLUVIAL_SYSTEM_HEURISTICSINKREMOVALPROCESSINGMODE_H
