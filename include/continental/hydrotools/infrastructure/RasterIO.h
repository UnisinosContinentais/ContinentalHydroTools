#ifndef CONTINENTAL_HYDROTOOLS_COMMAND_RASTERIO_H
#define CONTINENTAL_HYDROTOOLS_COMMAND_RASTERIO_H

#include "AbstractRasterIO.h"
#include "continental/datamanagement/Raster.h"
#include "../domain/FileCommand.h"

namespace continental {
namespace hydrotools {
namespace infrastructure {

template<class RasterType>
class RasterIO : public AbstractRasterIO<RasterType>
{
public:
    /// Construtor   
    RasterIO(domain::FileCommand pathFile) : AbstractRasterIO<RasterType>(pathFile){};

    void RasterIO::write(datamanagement::Raster<RasterType> &raster)
    {
        AbstractRasterIO<RasterType>::writeRasterIO(raster, getDatasetName(), 1);
    }

    bool RasterIO::exist()
    {
      return AbstractRasterIO<RasterType>::existRasterIO(getPathFile(), getDatasetName());
    }

    datamanagement::Raster<RasterType> RasterIO::read()
    {
        return AbstractRasterIO<RasterType>::readRasterIO(getPathFile(), getDatasetName()).second;
    }
};

} //infrastructure
} //hydrotools
} //continental

#endif // CONTINENTALPLUGIN_DAO_FLUVIAL_SYSTEM_CATCHMENTDELINEATIONDAO_H
