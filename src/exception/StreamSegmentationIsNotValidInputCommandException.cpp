#include "continental/hydrotools/exception/StreamSegmentationIsNotValidInputCommandException.h"

using namespace continental::hydrotools::exception;

StreamSegmentationIsNotValidInputCommandException::StreamSegmentationIsNotValidInputCommandException():
std::runtime_error("Dados inválidos para realizar o processo do Stream Segmentation!")
{

}
