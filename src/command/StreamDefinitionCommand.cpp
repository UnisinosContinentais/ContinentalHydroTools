#include "continental/hydrotools/command/StreamDefinitionCommand.h"
#include "continental/hydrotools/domain/StreamDefinitionCommandInput.h"
#include "continental/hydrotools/domain/FileCommand.h"
#include "continental/hydrotools/service/StreamDefinition.h"
#include "continental/datamanagement/Raster.h"
#include "continental/hydrotools/infrastructure/RasterIO.h"
#include "continental/hydrotools/util/HydroToolsUtil.h"
#include "continental/hydrotools/exception/StreamDefinitionProcessException.h"

using namespace std;
using namespace continental::hydrotools::domain;
using namespace continental::hydrotools::command;
using namespace continental::hydrotools::service;
using namespace continental::hydrotools::infrastructure;
using namespace continental::hydrotools::exception;

namespace continental {
namespace hydrotools {
namespace command {

StreamDefinitionCommand::StreamDefinitionCommand(StreamDefinitionCommandInput streamDefinitionCommandInput) :
m_streamDefinitionCommandInput(streamDefinitionCommandInput)
{

}

void StreamDefinitionCommand::execute()
{

        //Parametros de Entrada
        auto flowAccumulationInputFile = RasterIO<int>(m_streamDefinitionCommandInput.getFlowAccumulationInput());
        auto flowStreamDefinitionOutputFile = RasterIO<short>(m_streamDefinitionCommandInput.getStreamDefinitionOutput());

        auto flowAccumulationData = make_shared<datamanagement::Raster<int>>(flowAccumulationInputFile.read());
        double thresoldValue = m_streamDefinitionCommandInput.getThresoldValue();
        StreamDefinition::ThresholdType thresholdType =  m_streamDefinitionCommandInput.getThresholdType();

        //Prepara o objeto para processamento
        StreamDefinition streamDefinition;
        streamDefinition.setFlowAccumulation(flowAccumulationData, thresoldValue, thresholdType);
        //streamDefinition.validParameter();

        try
        {
            streamDefinition.defineStreams();
            //Grava o resultado do StreamDefinition
            flowStreamDefinitionOutputFile.write(*streamDefinition.getStreamDefinition().get());
        }
        catch (...)
        {
             throw exception::StreamDefinitionProcessException();
        }

}

}
}
}
