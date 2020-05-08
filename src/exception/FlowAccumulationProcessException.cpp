#include "continental/hydrotools/exception/FlowAccumulationProcessException.h"
#include "continental/hydrotools/constant/MensageConstant.h"

using namespace continental::hydrotools::exception;
using namespace continental::hydrotools::constant;

FlowAccumulationProcessException::FlowAccumulationProcessException() :
std::runtime_error(MensageConstant::FlowAccumulationProcessException)
{

}
