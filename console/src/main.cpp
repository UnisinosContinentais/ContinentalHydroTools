#include <continental/hydrotools/HeuristicSinkRemoval.h>
#include <memory>
#include <QString>

using namespace continental::hydrotools;
using namespace std;

int main(int argc, char **argv)
{
	QString inputDemFile = "D:\\Git\\ContinentalHydroTools\\ContinentalHydroTools\\assets\\rioSinos.asc";
	QString outputCorrectedDemFile = "D:\\Git\\ContinentalHydroTools\\ContinentalHydroTools\\assets\\rioSinos_sink.asc";
	QString outputFlowDirectionDemFile = "D:\\Git\\ContinentalHydroTools\\ContinentalHydroTools\\assets\\rioSinos_sink.asc";

	size_t maxOpenList = 1000000;
	size_t maxClosedList = 500000;
	float weightFunctionG = 2;
	auto processingAlgorithm = HeuristicSinkRemoval::ProcessingMode::MHS;
	auto sinkDestroy = make_unique<HeuristicSinkRemoval>(maxOpenList, maxClosedList, weightFunctionG, processingAlgorithm);
	sinkDestroy->readMDEdata(inputDemFile);
	sinkDestroy->removeSinks();
}
