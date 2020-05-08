#include "continental/hydrotools/exception/StreamDefinitionProcessException.h"
#include "continental/hydrotools/constant/MensageConstant.h"

using namespace continental::hydrotools::exception;
using namespace continental::hydrotools::constant;

StreamDefinitionProcessException::StreamDefinitionProcessException():
std::runtime_error(MensageConstant::StreamDefinitionProcessException)
{

}
