#include "continental/hydrotools/exception/StreamDefinitionIsNotValidInputCommandException.h"

using namespace continental::hydrotools::exception;

StreamDefinitionIsNotValidInputCommandException::StreamDefinitionIsNotValidInputCommandException(std::string  error):
std::runtime_error(error)
{
}
