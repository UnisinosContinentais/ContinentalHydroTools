#include "continental/hydrotools/command/StreamSegmentationCommand.h"
#include "continental/hydrotools/service/StreamSegmentation.h"
#include "continental/hydrotools/domain/StreamSegmentationCommandInput.h"
#include "continental/hydrotools/domain/FileCommand.h"
#include "continental/datamanagement/Raster.h"
#include "continental/hydrotools/infrastructure/RasterIO.h"
#include "continental/hydrotools/util/HydroToolsUtil.h"
#include "continental/hydrotools/exception/StreamSegmentationProcessException.h"

using namespace std;
using namespace continental::hydrotools::domain;
using namespace continental::hydrotools::command;
using namespace continental::hydrotools::service;
using namespace continental::hydrotools::infrastructure;
using namespace continental::hydrotools::exception;

namespace continental {
namespace hydrotools {
namespace command {

StreamSegmentationCommand::StreamSegmentationCommand(StreamSegmentationCommandInput streamSegmentationCommandInput):
m_streamSegmentationCommandInput(streamSegmentationCommandInput) {}

void StreamSegmentationCommand::execute()
{
    try
    {
        //Parametros de Entrada
        auto streamDefinitionInputFile = RasterIO<short>(m_streamSegmentationCommandInput.getStreamDefinitionInput());
        auto flowDirectionInputFile = RasterIO<short>(m_streamSegmentationCommandInput.getFlowDirectionInput());
        auto streamSegmentationOutputFile = RasterIO<short>(m_streamSegmentationCommandInput.getStreamSegmentationOutput());

        auto streamDefinitionData = make_shared<Raster<short>>(streamDefinitionInputFile.read());
        auto flowDirectionData = make_shared<Raster<short>>(flowDirectionInputFile.read());

        //Prepara o objeto para processamento
        StreamSegmentation streamSegmentation;
        streamSegmentation.setStreamDefinition(streamDefinitionData);
        streamSegmentation.setFlowDirection(flowDirectionData);
        streamSegmentation.segmentStreams();

        //Grava o resultado
        streamSegmentationOutputFile.write(*streamSegmentation.getStreamSegmentation().get());

    }
    catch (...)
    {
        throw exception::StreamSegmentationProcessException();
    }
}

}
}
}
