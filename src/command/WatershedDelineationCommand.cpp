#include "continental/hydrotools/command/WatershedDelineationCommand.h"
#include "continental/hydrotools/service/Catchment.h"
#include "continental/hydrotools/domain/WatershedDelineationCommandInput.h"
#include "continental/hydrotools/domain/FileCommand.h"
#include "continental/datamanagement/Raster.h"
#include "continental/hydrotools/infrastructure/RasterIO.h"
#include "continental/hydrotools/util/HydroToolsUtil.h"
#include "continental/hydrotools/exception/WatershedDelineationProcessException.h"

using namespace std;
using namespace continental::hydrotools::domain;
using namespace continental::hydrotools::command;
using namespace continental::hydrotools::service;
using namespace continental::hydrotools::infrastructure;
using namespace continental::hydrotools::exception;

namespace continental {
namespace hydrotools {
namespace command {

WatershedDelineationCommand::WatershedDelineationCommand(WatershedDelineationCommandInput watershedDelineationCommandInput)
:m_watershedDelineationCommandInput(watershedDelineationCommandInput)
{
}

void WatershedDelineationCommand::execute()
{
    try
    {
        auto watershedDelineationCommandOutputFile = RasterIO<short>(m_watershedDelineationCommandInput.getWatershedDelineationOutput());
        auto flowDirectionInputFile = RasterIO<short>(m_watershedDelineationCommandInput.getFlowDirectionInput());

        size_t m_row = m_watershedDelineationCommandInput.getRow();
        size_t m_col =m_watershedDelineationCommandInput.getCol();
        auto flowDirectionData = flowDirectionInputFile.read();

        Catchment catchmentCalculator;
        catchmentCalculator.setFlowDirection(std::make_shared<continental::datamanagement::Raster<short>>(flowDirectionData));
        catchmentCalculator.insertOutletByRowCol(m_row, m_col);
        catchmentCalculator.findWatersheds();
        watershedDelineationCommandOutputFile.write(*catchmentCalculator.getWaterShed());

    }
    catch (...)
    {
        throw exception::watershedDelineationProcessException;
    }
}

}
}
}
