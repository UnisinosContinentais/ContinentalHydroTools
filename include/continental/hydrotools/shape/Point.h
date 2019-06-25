#ifndef CONTINENTAL_HYDROTOOLS_SHAPE_POINT_H
#define CONTINENTAL_HYDROTOOLS_SHAPE_POINT_H

namespace continental
{
namespace hydrotools
{
namespace shape
{

template<typename T>
class Point
{
private:
T m_x;
T m_y;

public:
Point() : m_x(static_cast<T>(0)), m_y(static_cast<T>(0))
{
}

Point(T x, T y) : m_x(x), m_y(y)
{
}

void set(T x, T y)
{
    m_x = x;
    m_y = y;
}

T x()
{
    return m_x;
}

T y()
{
    return m_y;
}
};

}
}
}

#endif // CONTINENTAL_HYDROTOOLS_SHAPE_POINT_H
