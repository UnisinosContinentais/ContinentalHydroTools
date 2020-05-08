#include "continental/hydrotools/exception/CatchmentDelineationProcessException.h"
#include "continental/hydrotools/constant/MensageConstant.h"

using namespace continental::hydrotools::exception;
using namespace continental::hydrotools::constant;

CatchmentDelineationProcessException::CatchmentDelineationProcessException():
std::runtime_error(MensageConstant::CatchmentDelineationProcessException)
{

}
