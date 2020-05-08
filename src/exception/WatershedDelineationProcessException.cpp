#include "continental/hydrotools/exception/WatershedDelineationProcessException.h"
#include "continental/hydrotools/constant/MensageConstant.h"

using namespace continental::hydrotools::exception;
using namespace continental::hydrotools::constant;

WatershedDelineationProcessException::WatershedDelineationProcessException():
std::runtime_error(MensageConstant::WatershedDelineationProcessException)
{
}
