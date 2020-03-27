#include <continental/datamanagement/RasterFile.h>
#include <continental/hydrotools/service/HeuristicSinkRemoval.h>
#include <continental/hydrotools/service/FlowAccumulation.h>
#include <continental/hydrotools/service/StreamDefinition.h>
#include <continental/hydrotools/service/StreamSegmentation.h>
#include <continental/hydrotools/service/Catchment.h>
#include <memory>
#include <QString>
#include <QCryptographicHash>
#include <gtest/gtest.h>
#include <iostream>

using namespace continental::datamanagement;
using namespace continental::hydrotools::service;
using namespace std;

static QString basePath = "C:/Git/ContinentalHydroTools/ContinentalHydroToolsAssets/rioSinos";
static QString inputDemFile = basePath + "/rioSinos.asc";
static QString unitTestCorrectedFile = basePath + "/unit_rioSinos_sink.asc";
static QString unitTestFlowDirectionFile = basePath + "/unit_rioSinos_fdr.asc";
static QString unitTestFlowAccumulationFile = basePath + "/unit_rioSinos_fac.asc";
static QString unitTestStreamDefinitionFile = basePath + "/unit_rioSinos_str.asc";
static QString unitTestStreamSegmentationFile = basePath + "/unit_rioSinos_strseg.asc";
static QString unitTestWatershedDelineation = basePath + "/unit_rioSinos_wat.asc";
static QString unitTestCatchmentDelineation = basePath + "/unit_rioSinos_catch.asc";
static QString inputShapeFilePointOutletsSnap = basePath + "/Exutorio_snap.shp";

static QString correctResultCorrectedFile = basePath + "/rioSinos_sink.asc";
static QString correctResultFlowDirectionFile = basePath + "/rioSinos_fdr.asc";
static QString correctResultFlowAccumulationFile = basePath + "/rioSinos_fac.asc";
static QString correctResultStreamDefinitionFile = basePath + "/rioSinos_str.asc";
static QString correctResultStreamSegmentationFile = basePath + "/rioSinos_strseg.asc";
static QString correctResultWatershedDelineation = basePath + "/rioSinos_wat.asc";
static QString correctResultCatchmentDelineation = basePath + "/rioSinos_catch.asc";

QString getMd5OfFile(QString filePath)
{
    QFile file(filePath);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QByteArray content = file.readAll();

    return QString(QCryptographicHash::hash(content, QCryptographicHash::Md5).toHex());

}

TEST(ContinentalHydroToolsTest, SinkAndDestroy)
{
    size_t maxOpenList = 1000000;
    size_t maxClosedList = 500000;
    float weightFunctionG = 2;
    auto processingAlgorithm = HeuristicSinkRemoval<short>::ProcessingMode::MHS;
    auto sinkDestroy = make_unique<HeuristicSinkRemoval<short>>(maxOpenList, maxClosedList, weightFunctionG, processingAlgorithm);
    sinkDestroy->setDem(make_shared<Raster<short>>(RasterFile<short>::loadRasterByFile(inputDemFile)));
    sinkDestroy->removeSinks();

    RasterFile<short>::writeData(*sinkDestroy->getDem(), unitTestCorrectedFile);
    RasterFile<short>::writeData(*sinkDestroy->getFlowDirection(), unitTestFlowDirectionFile);
}

TEST(ContinentalHydroToolsTest, SinkAndDestroyCompare)
{
    EXPECT_EQ(getMd5OfFile(unitTestCorrectedFile), getMd5OfFile(correctResultCorrectedFile));
}

TEST(ContinentalHydroToolsTest, FlowDirectionCompare)
{
    EXPECT_EQ(getMd5OfFile(unitTestFlowDirectionFile), getMd5OfFile(correctResultFlowDirectionFile));
}

TEST(ContinentalHydroToolsTest, FlowAccumulation)
{
    auto flowDirectionData = make_shared<Raster<short>>(RasterFile<short>::loadRasterByFile(unitTestFlowDirectionFile));

    FlowAccumulation flowAccumulation;
    flowAccumulation.setFlowDirection(flowDirectionData);
    flowAccumulation.runoff();
    RasterFile<float>::writeData(*flowAccumulation.getFlowAccumulation().get(), unitTestFlowAccumulationFile);
}

TEST(ContinentalHydroToolsTest, FlowAccumulationCompare)
{
    EXPECT_EQ(getMd5OfFile(unitTestFlowAccumulationFile), getMd5OfFile(correctResultFlowAccumulationFile));
}

TEST(ContinentalHydroToolsTest, StreamDefinition)
{
    auto flowAccumulationData = make_shared<Raster<float>>(RasterFile<float>::loadRasterByFile(unitTestFlowAccumulationFile));

    StreamDefinition streamDefinition;
    streamDefinition.setFlowAccumulation(flowAccumulationData, 1000, StreamDefinition::ThresholdType::NumberOfCells);
    streamDefinition.defineStreams();
    RasterFile<short>::writeData(*streamDefinition.getStreamDefinition().get(), unitTestStreamDefinitionFile);
}

TEST(ContinentalHydroToolsTest, StreamDefinitionCompare)
{
    EXPECT_EQ(getMd5OfFile(unitTestStreamDefinitionFile), getMd5OfFile(correctResultStreamDefinitionFile));
}

TEST(ContinentalHydroToolsTest, StreamSegmentation)
{
    auto streamDefinitionData = make_shared<Raster<short>>(RasterFile<short>::loadRasterByFile(unitTestStreamDefinitionFile));
    auto flowDirectionData = make_shared<Raster<short>>(RasterFile<short>::loadRasterByFile(unitTestFlowDirectionFile));

    StreamSegmentation streamSegmentation;
    streamSegmentation.setStreamDefinition(streamDefinitionData);
    streamSegmentation.setFlowDirection(flowDirectionData);
    streamSegmentation.segmentStreams();
    RasterFile<short>::writeData(*streamSegmentation.getStreamSegmentation().get(), unitTestStreamSegmentationFile);
}

TEST(ContinentalHydroToolsTest, StreamSegmentationCompare)
{
    EXPECT_EQ(getMd5OfFile(unitTestStreamSegmentationFile), getMd5OfFile(correctResultStreamSegmentationFile));
}

TEST(ContinentalHydroToolsTest, WatershedDelineation)
{
    auto flowDirectionData = make_shared<Raster<short>>(RasterFile<short>::loadRasterByFile(unitTestFlowDirectionFile));

    Catchment catchment;
    catchment.setFlowDirection(flowDirectionData);
    // catchment.setPointOutlets(inputShapeFilePointOutletsSnap);
    catchment.insertOutletByRowCol(795, 209);
    catchment.findWatersheds();

    RasterFile<short>::writeData(*catchment.getWaterShed().get(), unitTestWatershedDelineation);
}

TEST(ContinentalHydroToolsTest, WatershedDelineationCompare)
{
    EXPECT_EQ(getMd5OfFile(unitTestWatershedDelineation), getMd5OfFile(correctResultWatershedDelineation));
}

TEST(ContinentalHydroToolsTest, CatchmentDelineation)
{
    auto flowDirectionData = make_shared<Raster<short>>(RasterFile<short>::loadRasterByFile(unitTestFlowDirectionFile));
    auto streamSegmentationData = make_shared<Raster<short>>(RasterFile<short>::loadRasterByFile(unitTestStreamSegmentationFile));

    Catchment catchment;
    catchment.setFlowDirection(flowDirectionData);
    catchment.setStreamSegmentation(streamSegmentationData);
    catchment.findWatersheds();

    RasterFile<short>::writeData(*catchment.getWaterShed().get(), unitTestCatchmentDelineation);
}

TEST(ContinentalHydroToolsTest, CatchmentDelineationCompare)
{
    EXPECT_EQ(getMd5OfFile(unitTestCatchmentDelineation), getMd5OfFile(correctResultCatchmentDelineation));
}

int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
