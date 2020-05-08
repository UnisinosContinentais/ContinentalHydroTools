#include "continental/hydrotools/exception/SinkDestroyProcessException.h"
#include "continental/hydrotools/constant/MensageConstant.h"

using namespace continental::hydrotools::exception;
using namespace continental::hydrotools::constant;

SinkDestroyProcessException::SinkDestroyProcessException() :
std::runtime_error(MensageConstant::SinkDestroyProcessException)
{

}
