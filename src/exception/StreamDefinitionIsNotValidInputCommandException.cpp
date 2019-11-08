#include "continental/hydrotools/exception/StreamDefinitionIsNotValidInputCommandException.h"

using namespace continental::hydrotools::exception;

StreamDefinitionIsNotValidInputCommandException::StreamDefinitionIsNotValidInputCommandException():
std::runtime_error("Dados inválidos para realizar o processo do Stream Definition!")
{
}
