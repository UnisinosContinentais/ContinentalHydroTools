#ifndef IPHYDRORASTERTOOLS_JUNCTIONCELL_H
#define IPHYDRORASTERTOOLS_JUNCTIONCELL_H

#include "continental/hydrotools/Cell.h"

namespace continental
{
namespace hydrotools
{
class JunctionCell : public Cell
{
    public:
        float ActualLength = 0;
        float SegmentValue = 0;
        //Public SegmentValue As Integer

        JunctionCell(short y, short x);
};
}
}

#endif // IPHYDRORASTERTOOLS_JUNCTIONCELL_H
