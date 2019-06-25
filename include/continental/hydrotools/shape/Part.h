#ifndef CONTINENTAL_HYDROTOOLS_SHAPE_PART_H
#define CONTINENTAL_HYDROTOOLS_SHAPE_PART_H

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

#endif // CONTINENTAL_HYDROTOOLS_SHAPE_PART_H
