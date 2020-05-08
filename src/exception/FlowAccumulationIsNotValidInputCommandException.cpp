#include "continental/hydrotools/exception/FlowAccumulationIsNotValidInputCommandException.h"
#include "continental/hydrotools/constant/MensageConstant.h"

using namespace continental::hydrotools::exception;
using namespace continental::hydrotools::constant;


FlowAccumulationIsNotValidInputCommandException::FlowAccumulationIsNotValidInputCommandException():
std::runtime_error(MensageConstant::InvalidArgumentsFlowAccumulation)
{

}
