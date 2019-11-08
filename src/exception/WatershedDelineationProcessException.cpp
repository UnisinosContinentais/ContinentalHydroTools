#include "continental/hydrotools/exception/WatershedDelineationProcessException.h"

using namespace continental::hydrotools::exception;

WatershedDelineationProcessException::WatershedDelineationProcessException():
std::runtime_error("O processo do Watershed Delineation não foi processado com sucesso!")
{
}
