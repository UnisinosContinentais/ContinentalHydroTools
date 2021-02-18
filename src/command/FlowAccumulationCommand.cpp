#include "continental/hydrotools/command/FlowAccumulation.h"
#include "continental/hydrotools/service/FlowAccumulation.h"
#include "continental/hydrotools/domain/FlowAccumulationCommandInput.h"
#include "continental/hydrotools/domain/FileCommand.h"
#include "continental/datamanagement/Raster.h"
#include "continental/hydrotools/infrastructure/RasterIO.h"
#include "continental/hydrotools/util/HydroToolsUtil.h"
#include "continental/hydrotools/exception/FlowAccumulationProcessException.h"

using namespace std;
using namespace continental::hydrotools::domain;
using namespace continental::hydrotools::command;
using namespace continental::hydrotools::service;
using namespace continental::hydrotools::infrastructure;
using namespace continental::hydrotools::exception;

namespace continental {
namespace hydrotools {
namespace command {

FlowAccumulationCommand::FlowAccumulationCommand(FlowAccumulationCommandInput flowAccumulationCommand) :
m_flowAccumulationCommand(flowAccumulationCommand)
{
}

void FlowAccumulationCommand::execute()
{
    try
    {
        //Parametros de Entrada
        auto flowDirectionInputFile = RasterIO<short>(m_flowAccumulationCommand.getFlowDirectionInput());
        auto flowccumulationOutputFile = RasterIO<int>(m_flowAccumulationCommand.getFlowAccumulationOutput());

        if(flowDirectionInputFile.exist())
        {
            auto flowDirectionData = make_shared<datamanagement::Raster<short>>(flowDirectionInputFile.read());

            //Prepara o objeto para processamento
            FlowAccumulation flowAccumulation;
            flowAccumulation.setFlowDirection(flowDirectionData);
            flowAccumulation.runoff();
            auto dataFlowAccumulation = *flowAccumulation.getFlowAccumulation().get();

            //Grava o resultado do FlowAccumulation
            flowccumulationOutputFile.write(dataFlowAccumulation);
        }
        else
        {
            throw exception::FlowAccumulationProcessException();
        }
    }
    catch (...)
    {
        throw exception::FlowAccumulationProcessException();
    }
}

}
}
}
