/*
* Developed by UNISINOS
* author: Luiz Felipe Bertoldi de Oliveira
* email: lbertoldio@unisinos.br
* date: January, 2019
*/

#ifndef CONTINENTAL_HYDROTOOLS_SHAPE_PART_H
#define CONTINENTAL_HYDROTOOLS_SHAPE_PART_H

#include <memory>

namespace continental
{
namespace hydrotools
{
namespace service
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
}

#endif // CONTINENTAL_HYDROTOOLS_SHAPE_PART_H
