#include "continental/hydrotools/exception/CatchmentDelineationIsNotValidInputCommandException.h"
#include "continental/hydrotools/constant/MensageConstant.h"

using namespace continental::hydrotools::exception;
using namespace continental::hydrotools::constant;

CatchmentDelineationIsNotValidInputCommandException::CatchmentDelineationIsNotValidInputCommandException():
std::runtime_error(MensageConstant::InvalidArgumentsCatchmentDelineation)
{

}
