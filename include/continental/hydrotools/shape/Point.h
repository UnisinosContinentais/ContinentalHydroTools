#ifndef IPHYDRORASTERTOOLS_SHAPE_POINT_H
#define IPHYDRORASTERTOOLS_SHAPE_POINT_H

namespace IPHydroRasterTools {
    namespace Shape {
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

#endif // IPHYDRORASTERTOOLS_SHAPE_POINT_H
