#include "continental/hydrotools/exception/StreamSegmentationProcessException.h"

using namespace continental::hydrotools::exception;

StreamSegmentationProcessException::StreamSegmentationProcessException(std::string error):
std::runtime_error(error)
{
}
