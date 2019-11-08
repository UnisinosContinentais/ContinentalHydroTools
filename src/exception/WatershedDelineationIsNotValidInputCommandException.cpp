#include "continental/hydrotools/exception/WatershedDelineationIsNotValidInputCommandException.h"

using namespace continental::hydrotools::exception;

WatershedDelineationIsNotValidInputCommandException::WatershedDelineationIsNotValidInputCommandException():
std::runtime_error("Dados inválidos para realizar o processo do Watershed Delineation!")
{

}
