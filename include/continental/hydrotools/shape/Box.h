#ifndef IPHYDRORASTERTOOLS_SHAPE_BOX_H
#define IPHYDRORASTERTOOLS_SHAPE_BOX_H

#include "continental/hydrotools/shape/Point.h"

namespace IPHydroRasterTools
{
    namespace Shape
    {
        template<typename T>
        class Box
        {
            private:
                T m_xMin;
                T m_yMin;
                T m_xMax;
                T m_yMax;
            public:
                Box(): m_xMin(static_cast<T>(0)), m_yMin(static_cast<T>(0)), m_xMax(static_cast<T>(0)), m_yMax(static_cast<T>(0))
                {
                }

                T Width() const
                {
                    return m_xMax - m_xMin;
                }

                T Height() const
                {
                    return m_yMax - m_yMin;
                }

                Point<T> Center() const
                {
                    T xoffset = ((m_xMax - m_xMin) / static_cast<T>(2));
                    T yoffset = ((m_yMax - m_yMin) / static_cast<T>(2));
                    return Point<T>(m_xMin + xoffset, m_yMin + yoffset);
                }

                void Grow(T delta)
                {
                    m_xMin -= delta;
                    m_xMax += delta;
                    m_yMin -= delta;
                    m_yMax += delta;
                }

                void Grow(T dx, T dy)
                {
                    m_xMin -= dx;
                    m_xMax += dx;
                    m_yMin -= dy;
                    m_yMax += dy;
                }

                void Move(T x, T y)
                {
                    m_xMin += x;
                    m_xMax += x;
                    m_yMin += y;
                    m_yMax += y;
                }

                void xMin(T xMin)
                {
                    m_xMin = xMin;
                }

                T xMin()
                {
                    return m_xMin;
                }

                void yMin(T yMin)
                {
                    m_yMin = yMin;
                }

                T yMin()
                {
                    return m_yMin;
                }

                void xMax(T xMax)
                {
                    m_xMax = xMax;
                }

                T xMax()
                {
                    return m_xMax;
                }

                void yMax(T yMax)
                {
                    m_yMax = yMax;
                }

                T yMax()
                {
                    return m_yMax;
                }
            };
    }
}

#endif // IPHYDRORASTERTOOLS_SHAPE_BOX_H
