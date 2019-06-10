#include <continental/datamanagement/RasterFile.h>
#include <continental/hydrotools/HeuristicSinkRemoval.h>
#include <continental/hydrotools/FlowAccumulation.h>
#include <continental/hydrotools/StreamDefinition.h>
#include <continental/hydrotools/StreamSegmentation.h>
#include <continental/hydrotools/Catchment.h>

#include <memory>
#include <QString>

using namespace continental::hydrotools;
using namespace continental::datamanagement;
using namespace std;

QString basePath = "D:/git/ContinentalHydroTools/ContinentalHydroToolsAssets";
QString inputDemFile = basePath + "/riosinos90arcgis.asc";
QString outputCorrectedFile = basePath + "/rioSinos90_sink.asc";
QString outputFlowDirectionFile = basePath + "/IPHrioSinos90_fdr.asc";
QString outputFlowAccumulationFile = basePath + "/rioSinos90_fac.asc";
QString outputStreamDefinitionFile = basePath + "/rioSinos90_str.asc";
QString outputStreamSegmentationDefinitionFile = basePath + "/rioSinos90_strseg.asc";
QString outputWatershedDelineation = basePath + "/rioSinos90_wat.asc";
QString inputShapeFilePointOutletsSnap = "";

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
    RasterFile<short>::writeData(*streamSegmentation.getStreamSegmentation().get(), outputStreamSegmentationDefinitionFile);
}

void watershedDelineation()
{
    auto flowDirectionData = make_shared<Raster<short>>(RasterFile<short>::loadRasterByFile(outputFlowDirectionFile));
    auto streamDefinitionData = make_shared<Raster<short>>(RasterFile<short>::loadRasterByFile(outputStreamDefinitionFile));

    Catchment catchement;
    catchement.setFlowDirection(flowDirectionData);
    // catchement.setPointOutlets(inputShapeFilePointOutletsSnap);
    catchement.findWatersheds();
    RasterFile<short>::writeData(*catchement.getWaterShed().get(), outputWatershedDelineation);
}

int main(int argc, char **argv)
{
    // sinkDestroy();
    // flowAccumulation();
    // streamDefinition();
    streamSegmention();

    return 0;
}
