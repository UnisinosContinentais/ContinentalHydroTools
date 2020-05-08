#include "continental/hydrotools/exception/WatershedDelineationIsNotValidInputCommandException.h"
#include "continental/hydrotools/constant/MensageConstant.h"

using namespace continental::hydrotools::exception;
using namespace continental::hydrotools::constant;

WatershedDelineationIsNotValidInputCommandException::WatershedDelineationIsNotValidInputCommandException():
std::runtime_error(MensageConstant::InvalidArgumentsWatershedDelineation)
{

}
