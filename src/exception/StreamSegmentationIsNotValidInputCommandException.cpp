#include "continental/hydrotools/exception/StreamSegmentationIsNotValidInputCommandException.h"
#include "continental/hydrotools/constant/MensageConstant.h"

using namespace continental::hydrotools::exception;
using namespace continental::hydrotools::constant;


StreamSegmentationIsNotValidInputCommandException::StreamSegmentationIsNotValidInputCommandException():
std::runtime_error(MensageConstant::InvalidArgumentsStreamSegmentation)
{

}
