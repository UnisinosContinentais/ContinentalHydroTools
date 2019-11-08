#include "continental/hydrotools/exception/CatchmentDelineationIsNotValidInputCommandException.h"

using namespace continental::hydrotools::exception;

CatchmentDelineationIsNotValidInputCommandException::CatchmentDelineationIsNotValidInputCommandException():
std::runtime_error("Dados inválidos para realizar o processo do Catchment Delineation!")
{

}
