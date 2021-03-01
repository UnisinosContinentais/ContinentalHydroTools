#include "continental/hydrotools/exception/FileNotFoundException.h"
#include "continental/hydrotools/constant/MensageConstant.h"

using namespace continental::hydrotools::constant;

namespace continental {
namespace hydrotools {
namespace exception {

FileNotFoundException::FileNotFoundException() :
    std::runtime_error(MensageConstant::FileNotFound)
{

}

} // namespace exception
} // namespace hydrotools
} // namespace continental
