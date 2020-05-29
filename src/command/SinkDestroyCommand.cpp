#include "continental/hydrotools/command/SinkDestroyCommand.h"
#include "continental/hydrotools/service/HeuristicSinkRemoval.h"
#include "continental/hydrotools/service/HeuristicSinkRemovalUtil.h"
#include "continental/hydrotools/domain/HeuristicSinkRemovalCommandInput.h"
#include "continental/hydrotools/domain/FileCommand.h"
#include "continental/datamanagement/Raster.h"
#include "continental/hydrotools/infrastructure/RasterIO.h"
#include "continental/hydrotools/util/HydroToolsUtil.h"
#include "continental/hydrotools/exception/SinkDestroyProcessException.h"

using namespace std;
using namespace continental::hydrotools::domain;
using namespace continental::hydrotools::command;
using namespace continental::hydrotools::service;
using namespace continental::hydrotools::infrastructure;
using namespace continental::hydrotools::exception;

namespace continental {
namespace hydrotools {
namespace command {

SinkDestroyCommand::SinkDestroyCommand(HeuristicSinkRemovalCommandInput sinkDestroyCommandInput):
m_heuristicSinkRemovalCommandInput(sinkDestroyCommandInput)
{
}

void SinkDestroyCommand::execute()
{
    try
    {
        //Parametros de Entrada
        auto rasterInputFile = RasterIO<float>(m_heuristicSinkRemovalCommandInput.getDemInput());
        auto outputCorrectedFile = RasterIO<float>(m_heuristicSinkRemovalCommandInput.getSinkDestroyOutput());
        auto outputFlowDirectionFile = RasterIO<short>(m_heuristicSinkRemovalCommandInput.getFlowDirectionOutput());

        size_t maxOpenList = m_heuristicSinkRemovalCommandInput.getMaxOpenList();
        size_t maxClosedList = m_heuristicSinkRemovalCommandInput.getMaxClosedList();
        float weightFunctionG = m_heuristicSinkRemovalCommandInput.getWeightFunctionG();

        auto processingAlgorithm = m_heuristicSinkRemovalCommandInput.getProcessingAlgorithm();

        if(rasterInputFile.exist())
        {
            //Prepara o objeto para processamento
            auto sinkDestroy = make_unique<HeuristicSinkRemoval<float>>(maxOpenList, maxClosedList, weightFunctionG, processingAlgorithm);
            sinkDestroy->setDem(make_shared<datamanagement::Raster<float>>(rasterInputFile.read()));
            sinkDestroy->removeSinks();

            //Grava o resultado
            outputCorrectedFile.write(*sinkDestroy->getDem());
            outputFlowDirectionFile.write(*sinkDestroy->getFlowDirection());
        }
        else
        {
            throw SinkDestroyProcessException();
        }
    }
    catch(std::exception &e)
    {
        throw SinkDestroyProcessException();
    }
}

}
}
}
