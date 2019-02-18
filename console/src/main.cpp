#include <continental/datamanagement/RasterFile.h>
#include <continental/hydrotools/HeuristicSinkRemoval.h>
#include <memory>
#include <QString>

using namespace continental::hydrotools;
using namespace continental::datamanagement;
using namespace std;

int main(int argc, char **argv)
{
	QString inputDemFile = "D:\\Git\\ContinentalHydroTools\\ContinentalHydroTools\\assets\\rioSinos.asc";
    QString outputCorrectedDemFile = "D:\\Git\\ContinentalHydroTools\\ContinentalHydroTools\\assets\\cplusplus_rioSinos_sink.asc";
    QString outputFlowDirectionDemFile = "D:\\Git\\ContinentalHydroTools\\ContinentalHydroTools\\assets\\cplusplus_rioSinos_fdr.asc";

	size_t maxOpenList = 1000000;
	size_t maxClosedList = 500000;
	float weightFunctionG = 2;
	auto processingAlgorithm = HeuristicSinkRemoval::ProcessingMode::MHS;
	auto sinkDestroy = make_unique<HeuristicSinkRemoval>(maxOpenList, maxClosedList, weightFunctionG, processingAlgorithm);
	sinkDestroy->readMDEdata(inputDemFile);
    sinkDestroy->removeSinks();

    RasterFile<short>::writeData(sinkDestroy->getMDE(), outputCorrectedDemFile);
    RasterFile<short>::writeData(sinkDestroy->getFlowDirection(), outputFlowDirectionDemFile);
}
