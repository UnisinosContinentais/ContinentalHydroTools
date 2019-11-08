#include "continental/hydrotools/exception/SinkDestroyIsNotValidInputCommandException.h"

using namespace continental::hydrotools::exception;

SinkDestroyIsNotValidInputCommandException::SinkDestroyIsNotValidInputCommandException():
std::runtime_error("Dados inválidos para realizar o processo do Sink Destroy!")
{
}
