#ifndef CONTINENTAL_HYDROTOOLS_COMMAND_RASTERIO_H
#define CONTINENTAL_HYDROTOOLS_COMMAND_RASTERIO_H

#include "AbstractRasterIO.h"
#include "continental/datamanagement/Raster.h"
#include "../domain/FileCommand.h"

using namespace continental::hydrotools::domain;
using namespace continental::datamanagement;

namespace continental {
namespace hydrotools {
namespace infrastructure {

template<class RasterType>
class RasterIO : public AbstractRasterIO<RasterType>
{
public:
    /// Construtor   
    RasterIO(FileCommand pathFile) : AbstractRasterIO<RasterType>(pathFile){};

    void RasterIO::write(Raster<RasterType> &raster)
    {
        AbstractRasterIO<RasterType>::writeRasterIO(raster, getDatasetName(), 1);
    }

    bool RasterIO::exist()
    {
      return AbstractRasterIO<RasterType>::existRasterIO(getPathFile(), getDatasetName());
    }

    Raster<RasterType> RasterIO::read()
    {
        return AbstractRasterIO<RasterType>::readRasterIO(getPathFile(), getDatasetName()).second;
    }
};

} //infrastructure
} //hydrotools
} //continental

#endif // CONTINENTALPLUGIN_DAO_FLUVIAL_SYSTEM_CATCHMENTDELINEATIONDAO_H
