/*
* Developed by UNISINOS - Projeto Continentais and Petrobras
* author: Luiz Felipe Bertoldi de Oliveira
* email: lbertoldio@unisinos.br
* date: June, 2019
*/
#ifndef CONTINENTALPLUGIN_DOMAIN_FLUVIAL_SYSTEM_STREAMDEFINITIONTHRESHOLDTYPE_H
#define CONTINENTALPLUGIN_DOMAIN_FLUVIAL_SYSTEM_STREAMDEFINITIONTHRESHOLDTYPE_H

namespace continental {
namespace domain {
namespace fluvialsystem {
namespace hydrotools {

enum class StreamDefinitionThresholdType
{
    NumberOfCells = 0,
    PercentualOfMaximumCells = 1,
    Area = 2 //km²
};

} // namespace hydrotools
} //fluvialsystem
} //domain
} //continental

#endif // CONTINENTALPLUGIN_DOMAIN_FLUVIAL_SYSTEM_STREAMDEFINITIONTHRESHOLDTYPE_H
