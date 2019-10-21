
#include <continental/hydrotools/HeuristicSinkRemoval.h>
#include <memory>
#include <QString>
#include <gtest/gtest.h>

using namespace continental::hydrotools;
using namespace std;

TEST(ContinentalHydroToolsTest, SinkAndDestroy)
{
    QString inputDemFile = "D:\\Git\\ContinentalHydroTools\\ContinentalHydroTools\\assets\\rioSinos.asc";
    QString outputCorrectedDemFile = "D:\\Git\\ContinentalHydroTools\\ContinentalHydroTools\\assets\\rioSinos_sink.asc";
    QString outputFlowDirectionDemFile = "D:\\Git\\ContinentalHydroTools\\ContinentalHydroTools\\assets\\rioSinos_sink.asc";

    size_t maxOpenList = 1000000;
    size_t maxClosedList = 500000;
    float weightFunctionG = 2;
    HeuristicSinkRemoval::ProcessingMode processingAlgorithm = HeuristicSinkRemoval::ProcessingMode::MHS;
    auto sinkDestroy = make_unique<HeuristicSinkRemoval>(maxOpenList, maxClosedList, weightFunctionG, processingAlgorithm);
    sinkDestroy->readMDEdata(inputDemFile);
    sinkDestroy->removeSinks();
}

int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}




int main(int argc, char **argv)
{
	CatchmentCommandInput inputCatchment = new CatchmentCommandInput();
	inputCatchment->
	InputHandler in;
	
	Command *c = new CatchmentCommand(&inputCatchment);
	in.SetCommand(c);
	in.DoCommand();
		
	return 0;
}
