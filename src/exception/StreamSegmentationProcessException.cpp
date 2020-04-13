#include "continental/hydrotools/exception/StreamSegmentationProcessException.h"

using namespace continental::hydrotools::exception;

StreamSegmentationProcessException::StreamSegmentationProcessException(string error):
std::runtime_error(error)
{
}
