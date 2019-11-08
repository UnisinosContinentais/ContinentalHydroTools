#include "continental/hydrotools/exception/CatchmentDelineationProcessException.h"

using namespace continental::hydrotools::exception;

CatchmentDelineationProcessException::CatchmentDelineationProcessException():
std::runtime_error("O processo do Catchment Delineation não foi processado com sucesso!")
{

}
