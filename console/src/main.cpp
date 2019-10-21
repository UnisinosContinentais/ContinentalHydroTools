#include <continental/datamanagement/RasterFile.h>
#include <continental/hydrotools/service/HeuristicSinkRemoval.h>
#include <continental/hydrotools/service/FlowAccumulation.h>
#include <continental/hydrotools/service/StreamDefinition.h>
#include <continental/hydrotools/service/StreamSegmentation.h>
#include <continental/hydrotools/service/Catchment.h>

#include <memory>
#include <QString>

using namespace continental::hydrotools::service;
using namespace continental::datamanagement;
using namespace std;

static QString basePath = "D:/git/ContinentalHydroTools/ContinentalHydroToolsAssets/rioSinos";
static QString inputDemFile = basePath + "/rioSinos.asc";
static QString outputCorrectedFile = basePath + "/rioSinos_sink.asc";
static QString outputFlowDirectionFile = basePath + "/rioSinos_fdr.asc";
static QString outputFlowAccumulationFile = basePath + "/rioSinos_fac.asc";
static QString outputStreamDefinitionFile = basePath + "/rioSinos_str.asc";
static QString outputStreamSegmentationFile = basePath + "/rioSinos_strseg.asc";
static QString outputWatershedDelineation = basePath + "/rioSinos_wat.asc";
static QString outputCatchmentDelineation = basePath + "/rioSinos_catch.asc";
static QString inputShapeFilePointOutletsSnap = basePath + "/Exutorio_snap.shp";

void sinkDestroy()
{
    size_t maxOpenList = 1000000;
    size_t maxClosedList = 500000;
    float weightFunctionG = 2;
    auto processingAlgorithm = HeuristicSinkRemoval::ProcessingMode::MHS;
    auto sinkDestroy = make_unique<HeuristicSinkRemoval>(maxOpenList, maxClosedList, weightFunctionG, processingAlgorithm);
    sinkDestroy->setDem(make_shared<Raster<short>>(RasterFile<short>::loadRasterByFile(inputDemFile)));
    sinkDestroy->removeSinks();

    RasterFile<short>::writeData(*sinkDestroy->getDem(), outputCorrectedFile);
    RasterFile<short>::writeData(*sinkDestroy->getFlowDirection(), outputFlowDirectionFile);
}

void flowAccumulation()
{
    auto flowDirectionData = make_shared<Raster<short>>(RasterFile<short>::loadRasterByFile(outputFlowDirectionFile));

    FlowAccumulation flowAccumulation;
    flowAccumulation.setFlowDirection(flowDirectionData);
    flowAccumulation.runoff();
    RasterFile<float>::writeData(*flowAccumulation.getFlowAccumulation().get(), outputFlowAccumulationFile);
}

void streamDefinition()
{
    auto flowAccumulationData = make_shared<Raster<float>>(RasterFile<float>::loadRasterByFile(outputFlowAccumulationFile));

    StreamDefinition streamDefinition;
    streamDefinition.setFlowAccumulation(flowAccumulationData, 3000, StreamDefinition::ThresholdType::NumberOfCells);
    streamDefinition.defineStreams();
    RasterFile<short>::writeData(*streamDefinition.getStreamDefinition().get(), outputStreamDefinitionFile);
}

void streamSegmention()
{
    auto streamDefinitionData = make_shared<Raster<short>>(RasterFile<short>::loadRasterByFile(outputStreamDefinitionFile));
    auto flowDirectionData = make_shared<Raster<short>>(RasterFile<short>::loadRasterByFile(outputFlowDirectionFile));

    StreamSegmentation streamSegmentation;
    streamSegmentation.setStreamDefinition(streamDefinitionData);
    streamSegmentation.setFlowDirection(flowDirectionData);
    streamSegmentation.segmentStreams();
    RasterFile<short>::writeData(*streamSegmentation.getStreamSegmentation().get(), outputStreamSegmentationFile);
}

void watershedDelineation()
{
    auto flowDirectionData = make_shared<Raster<short>>(RasterFile<short>::loadRasterByFile(outputFlowDirectionFile));

    Catchment catchment;
    catchment.setFlowDirection(flowDirectionData);
    // catchment.setPointOutlets(inputShapeFilePointOutletsSnap);
    catchment.insertOutletByRowCol(795, 209);
    catchment.findWatersheds();

    RasterFile<short>::writeData(*catchment.getWaterShed().get(), outputWatershedDelineation);
}

void catchmentDelineation()
{
    auto flowDirectionData = make_shared<Raster<short>>(RasterFile<short>::loadRasterByFile(outputFlowDirectionFile));
    auto streamSegmentationData = make_shared<Raster<short>>(RasterFile<short>::loadRasterByFile(outputStreamSegmentationFile));

    Catchment catchment;
    catchment.setFlowDirection(flowDirectionData);
    catchment.setStreamSegmentation(streamSegmentationData);
    catchment.findWatersheds();

    RasterFile<short>::writeData(*catchment.getWaterShed().get(), outputCatchmentDelineation);
}

int main(int argc, char **argv)
{
    Q_UNUSED(argc);
    Q_UNUSED(argv);

    // sinkDestroy();
    // flowAccumulation();
    // streamDefinition();
    // streamSegmention();
    watershedDelineation();
    // catchmentDelineation();

    return 0;
}
