#include "continental/hydrotools/exception/StreamDefinitionIsNotValidInputCommandException.h"
#include "continental/hydrotools/constant/MensageConstant.h"

using namespace continental::hydrotools::exception;
using namespace continental::hydrotools::constant;

StreamDefinitionIsNotValidInputCommandException::StreamDefinitionIsNotValidInputCommandException(std::string  error):
std::runtime_error(error)
{
}
