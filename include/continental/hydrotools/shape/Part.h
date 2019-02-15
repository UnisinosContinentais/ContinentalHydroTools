#ifndef IPHYDRORASTERTOOLS_SHAPE_PART_H
#define IPHYDRORASTERTOOLS_SHAPE_PART_H

#include <memory>

namespace continental
{
namespace hydrotools
{
namespace shape
{

class Part
{
public:
int type;
int offset;
size_t length;

Part() : type(0), offset(0), length(0) {}
};

}
}
}

#endif // IPHYDRORASTERTOOLS_SHAPE_PART_H
