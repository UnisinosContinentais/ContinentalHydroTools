#include "continental/hydrotools/exception/SinkDestroyIsNotValidInputCommandException.h"
#include "continental/hydrotools/constant/MensageConstant.h"

using namespace continental::hydrotools::exception;
using namespace continental::hydrotools::constant;


SinkDestroyIsNotValidInputCommandException::SinkDestroyIsNotValidInputCommandException():
std::runtime_error(MensageConstant::InvalidArgumentsSinkDestroy)
{
}
