/*
* Developed by UNISINOS
* author: Luiz Felipe Bertoldi de Oliveira
* email: lbertoldio@unisinos.br
* date: January, 2019
*/

#ifndef CONTINENTAL_HYDROTOOLS_JUNCTIONCELL_H
#define CONTINENTAL_HYDROTOOLS_JUNCTIONCELL_H

#include "continental/hydrotools/service/Cell.h"

namespace continental
{
namespace hydrotools
{
namespace service
{
class JunctionCell : public Cell
{
    public:
        float ActualLength = 0;
        float SegmentValue = 0;
        //Public SegmentValue As Integer

        JunctionCell(size_t y, size_t x);
};

}
}
}

#endif // CONTINENTAL_HYDROTOOLS_JUNCTIONCELL_H
