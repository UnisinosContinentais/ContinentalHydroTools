#include "continental/hydrotools/command/CatchmentCommand.h"
#include "continental/hydrotools/service/Catchment.h"
#include "continental/hydrotools/domain/CatchmentCommandInput.h"
#include "continental/hydrotools/domain/FileCommand.h"
#include "continental/datamanagement/Raster.h"
#include "continental/hydrotools/infrastructure/RasterIO.h"
#include "continental/hydrotools/util/HydroToolsUtil.h"
#include "continental/hydrotools/exception/CatchmentDelineationProcessException.h"

using namespace std;
using namespace continental::hydrotools::domain;
using namespace continental::hydrotools::command;
using namespace continental::hydrotools::service;
using namespace continental::hydrotools::infrastructure;
using namespace continental::hydrotools::exception;

namespace continental {
namespace hydrotools {
namespace command {

CatchmentCommand::CatchmentCommand(CatchmentCommandInput catchmentCommandInput) :
m_catchmentCommandInput(catchmentCommandInput)
{

}
void CatchmentCommand::execute()
{
    try
    {
        //Parametros de Entrada
        auto flowDirectionInputFile = RasterIO<short>(m_catchmentCommandInput.getFlowDirectionInput());
        auto streamSegmentationInputFile = RasterIO<short>(m_catchmentCommandInput.getStreamSegmentationInput());
        auto catchmentDelineationOutputFile = RasterIO<short>(m_catchmentCommandInput.getCatchmentDelineationOutput());

        auto flowDirectionData = std::make_shared<Raster<short>>(flowDirectionInputFile.read());
        auto streamSegmentationData  = std::make_shared<Raster<short>>(streamSegmentationInputFile.read());

        //Prepara o objeto para processamento
        Catchment catchment;
        catchment.setFlowDirection(flowDirectionData);
        catchment.setStreamSegmentation(streamSegmentationData);
        catchment.findWatersheds();

        //Grava o resultado do StreamDefinition
        catchmentDelineationOutputFile.write(*catchment.getWaterShed().get());
    }
    catch (...)
    {
        throw exception::CatchmentDelineationProcessException();
    }
}

}
}
}
