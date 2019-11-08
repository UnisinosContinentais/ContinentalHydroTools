#include "continental/hydrotools/exception/StreamDefinitionProcessException.h"

using namespace continental::hydrotools::exception;

StreamDefinitionProcessException::StreamDefinitionProcessException():
std::runtime_error("O processo do Stream Definition não foi processado com sucesso!")
{

}
