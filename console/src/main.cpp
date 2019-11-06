#include <iostream>
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

static QString basePath = "C:/git/ContinentalHydroTools/ContinentalHydroToolsAssets/rioSinos/";
static QString baseFilename = "rioSinosFloat";
static QString inputDemFile = basePath + baseFilename + ".asc";
static QString outputCorrectedFile = basePath + baseFilename + "_sink.asc";
static QString outputFlowDirectionFile = basePath + baseFilename + "_fdr.asc";
static QString outputFlowAccumulationFile = basePath + baseFilename + "_fac.asc";
static QString outputStreamDefinitionFile = basePath + baseFilename + "_str.asc";
static QString outputStreamSegmentationFile = basePath + baseFilename + "_strseg.asc";
static QString outputWatershedDelineation = basePath + baseFilename + "_wat.asc";
static QString outputCatchmentDelineation = basePath + baseFilename + "_catch.asc";
static QString inputShapeFilePointOutletsSnap = basePath + "/Exutorio_snap.shp";

void sinkDestroy()
{
    size_t maxOpenList = 1000000;
    size_t maxClosedList = 500000;
    float weightFunctionG = 2;
    auto processingAlgorithm = HeuristicSinkRemoval<float>::ProcessingMode::MHS;
    auto sinkDestroy = make_unique<HeuristicSinkRemoval<float>>(maxOpenList, maxClosedList, weightFunctionG, processingAlgorithm);
	auto dem = make_shared<Raster<float>>(RasterFile<float>::loadRasterByFile(inputDemFile));
	sinkDestroy->setDem(dem);
    sinkDestroy->removeSinks();

    RasterFile<float>::writeData(*sinkDestroy->getDem(), outputCorrectedFile);
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
    streamDefinition.setFlowAccumulation(flowAccumulationData, 1000, StreamDefinition::ThresholdType::NumberOfCells);
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
    auto streamSegmentationData = make_shared<Raster<short>>(RasterFile<short>::loadRasterByFile(outputCatchmentDelineation));

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

	/*try
	{*/
		sinkDestroy();
		flowAccumulation();
		streamDefinition();
		streamSegmention();
		// catchmentDelineation();
		// watershedDelineation();
	/*}
	catch (std::exception e)
	{
		std::cout << e.what() << endl;
	}*/

    return 0;
}
