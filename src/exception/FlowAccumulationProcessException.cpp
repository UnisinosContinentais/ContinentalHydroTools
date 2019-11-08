#include "continental/hydrotools/exception/FlowAccumulationProcessException.h"

using namespace  continental::hydrotools::exception;

FlowAccumulationProcessException::FlowAccumulationProcessException() :
std::runtime_error("O processo do Flow Accumulation não foi processado com sucesso!")
{

}
