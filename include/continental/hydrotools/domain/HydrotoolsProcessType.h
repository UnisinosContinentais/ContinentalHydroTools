/*
* Developed by UNISINOS - Projeto Continentais and Petrobras
* author: Luiz Felipe Bertoldi de Oliveira
* email: lbertoldio@unisinos.br
* date: June, 2019
*/
#ifndef CONTINENTALPLUGIN_DOMAIN_FLUVIAL_SYSTEM_HEURISTICSINKREMOVALPROCESSINGMODE_H
#define CONTINENTALPLUGIN_DOMAIN_FLUVIAL_SYSTEM_HEURISTICSINKREMOVALPROCESSINGMODE_H

namespace continental {
namespace hydrotools {
namespace domain {

typedef enum
{
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
