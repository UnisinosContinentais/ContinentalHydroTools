#include "continental/hydrotools/exception/StreamSegmentationProcessException.h"

using namespace continental::hydrotools::exception;

StreamSegmentationProcessException::StreamSegmentationProcessException():
std::runtime_error("O processo do Stream Segmentation não foi processado com sucesso!")
{
}
