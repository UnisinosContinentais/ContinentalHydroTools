#include "continental/hydrotools/exception/SinkDestroyProcessException.h"
#include "continental/hydrotools/constant/MensageConstant.h"

using namespace continental::hydrotools::constant;

namespace continental {
namespace hydrotools {
namespace exception {

SinkDestroyProcessException::SinkDestroyProcessException() :
std::runtime_error(MensageConstant::SinkDestroyProcessException)
{

}

}
}
}
