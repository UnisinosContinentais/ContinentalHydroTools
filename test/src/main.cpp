#include <continental/datamanagement/RasterFile.h>
#include <continental/hydrotools/service/HeuristicSinkRemoval.h>
#include <continental/hydrotools/service/FlowAccumulation.h>
#include <continental/hydrotools/service/StreamDefinition.h>
#include <continental/hydrotools/service/StreamSegmentation.h>
#include <continental/hydrotools/service/Catchment.h>
#include <QString>
#include <QCryptographicHash>
#include <gtest/gtest.h>

using namespace continental::datamanagement;
using namespace continental::hydrotools::service;

const static QString inputDemFile = "rioSinos.asc";

const static QString unitTestCorrectedFile = "unit_rioSinos_sink.asc";
const static QString unitTestFlowDirectionFile = "unit_rioSinos_fdr.asc";
const static QString unitTestFlowAccumulationFile = "unit_rioSinos_fac.asc";
const static QString unitTestStreamDefinitionFile = "unit_rioSinos_str.asc";
const static QString unitTestStreamSegmentationFile = "unit_rioSinos_strseg.asc";
const static QString unitTestWatershedDelineation = "unit_rioSinos_wat.asc";
const static QString unitTestCatchmentDelineation = "unit_rioSinos_catch.asc";

const static QString correctResultCorrectedFile = "rioSinos_sink.asc";
const static QString correctResultFlowDirectionFile = "rioSinos_fdr.asc";
const static QString correctResultFlowAccumulationFile = "rioSinos_fac.asc";
const static QString correctResultStreamDefinitionFile = "rioSinos_str.asc";
const static QString correctResultStreamSegmentationFile = "rioSinos_strseg.asc";
const static QString correctResultWatershedDelineation = "rioSinos_wat.asc";
const static QString correctResultCatchmentDelineation = "rioSinos_catch.asc";

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
    double weightFunctionG = 2;
    auto processingAlgorithm = HeuristicSinkRemoval<short>::ProcessingMode::MHS;
    auto sinkDestroy = std::make_unique<HeuristicSinkRemoval<short>>(maxOpenList, maxClosedList, weightFunctionG, processingAlgorithm);
    sinkDestroy->setDem(std::make_shared<Raster<short>>(RasterFile<short>::loadRasterByFile(inputDemFile)));
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
    auto flowDirectionData = std::make_shared<Raster<short>>(RasterFile<short>::loadRasterByFile(unitTestFlowDirectionFile));

    FlowAccumulation flowAccumulation;
    flowAccumulation.setFlowDirection(flowDirectionData);
    flowAccumulation.runoff();
    RasterFile<int>::writeData(*flowAccumulation.getFlowAccumulation(), unitTestFlowAccumulationFile);
}

TEST(ContinentalHydroToolsTest, FlowAccumulationCompare)
{
    EXPECT_EQ(getMd5OfFile(unitTestFlowAccumulationFile), getMd5OfFile(correctResultFlowAccumulationFile));
}

TEST(ContinentalHydroToolsTest, StreamDefinition)
{
    auto flowAccumulationData = std::make_shared<Raster<int>>(RasterFile<int>::loadRasterByFile(correctResultFlowAccumulationFile));

    StreamDefinition streamDefinition;
    streamDefinition.setFlowAccumulation(flowAccumulationData, 3000, StreamDefinition::ThresholdType::NumberOfCells);
    streamDefinition.defineStreams();
    RasterFile<short>::writeData(*streamDefinition.getStreamDefinition(), unitTestStreamDefinitionFile);
}

TEST(ContinentalHydroToolsTest, StreamDefinitionCompare)
{
    EXPECT_EQ(getMd5OfFile(unitTestStreamDefinitionFile), getMd5OfFile(correctResultStreamDefinitionFile));
}

TEST(ContinentalHydroToolsTest, StreamSegmentation)
{
    auto streamDefinitionData = std::make_shared<Raster<short>>(RasterFile<short>::loadRasterByFile(unitTestStreamDefinitionFile));
    auto flowDirectionData = std::make_shared<Raster<short>>(RasterFile<short>::loadRasterByFile(unitTestFlowDirectionFile));

    StreamSegmentation streamSegmentation;
    streamSegmentation.setStreamDefinition(streamDefinitionData);
    streamSegmentation.setFlowDirection(flowDirectionData);
    streamSegmentation.segmentStreams();
    RasterFile<short>::writeData(*streamSegmentation.getStreamSegmentation(), unitTestStreamSegmentationFile);
}

TEST(ContinentalHydroToolsTest, StreamSegmentationCompare)
{
    EXPECT_EQ(getMd5OfFile(unitTestStreamSegmentationFile), getMd5OfFile(correctResultStreamSegmentationFile));
}

TEST(ContinentalHydroToolsTest, WatershedDelineation)
{
    auto flowDirectionData = std::make_shared<Raster<short>>(RasterFile<short>::loadRasterByFile(unitTestFlowDirectionFile));

    Catchment catchment;
    catchment.setFlowDirection(flowDirectionData);
    catchment.insertOutletByRowCol(795, 209);
    catchment.findWatersheds();

    RasterFile<short>::writeData(*catchment.getWaterShed(), unitTestWatershedDelineation);
}

TEST(ContinentalHydroToolsTest, WatershedDelineationCompare)
{
    EXPECT_EQ(getMd5OfFile(unitTestWatershedDelineation), getMd5OfFile(correctResultWatershedDelineation));
}

TEST(ContinentalHydroToolsTest, CatchmentDelineation)
{
    auto flowDirectionData = std::make_shared<Raster<short>>(RasterFile<short>::loadRasterByFile(unitTestFlowDirectionFile));
    auto streamSegmentationData = std::make_shared<Raster<short>>(RasterFile<short>::loadRasterByFile(unitTestStreamSegmentationFile));

    Catchment catchment;
    catchment.setFlowDirection(flowDirectionData);
    catchment.setStreamSegmentation(streamSegmentationData);
    catchment.findWatersheds();

    RasterFile<short>::writeData(*catchment.getWaterShed(), unitTestCatchmentDelineation);
}

TEST(ContinentalHydroToolsTest, CatchmentDelineationCompare)
{
    EXPECT_EQ(getMd5OfFile(unitTestCatchmentDelineation), getMd5OfFile(correctResultCatchmentDelineation));
}

int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  // ::testing::GTEST_FLAG(filter) = "*StreamDefinition*";
  return RUN_ALL_TESTS();
}
