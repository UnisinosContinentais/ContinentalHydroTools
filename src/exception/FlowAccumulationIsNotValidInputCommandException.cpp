#include "continental/hydrotools/exception/FlowAccumulationIsNotValidInputCommandException.h"

using namespace  continental::hydrotools::exception;

FlowAccumulationIsNotValidInputCommandException::FlowAccumulationIsNotValidInputCommandException():
std::runtime_error("Dados inválidos para realizar o processo do Catchment Delineation!")
{

}
