#include <QString>
#include "continental/hydrotools/shapeobject.h"

namespace IPHydroRasterTools
{

    ShapeObject::ShapeObject() : m_index(-1), m_type(0), m_vertexCount(0), m_vertices(0)
    {
    }

    ShapeObject::ShapeObject(const SHPObject* obj) : m_index(-1), m_type(0), m_vertexCount(0), m_vertices(0)
    {
        assign(obj);
    }

    ShapeObject::ShapeObject(const ShapeObject &other) : m_index(-1), m_type(0), m_vertexCount(0), m_vertices(0)
    {
        assign(other);
    }

    ShapeObject& ShapeObject::operator = (const ShapeObject& other)
    {
        assign(other);
        return *this;
    }

    void ShapeObject::destroy()
    {
        if (m_vertexCount > 0)
        {
            m_vertices->clear();
            m_vertexCount = 0;
        }
        m_type = 0;
        m_index = -1;
    }

    void ShapeObject::assign(const SHPObject* obj)
    {
        if (obj != 0) {
            if (m_index >= 0) {
                destroy();
            }
            m_index = obj->nShapeId;
            m_type = obj->nSHPType;
            m_vertexCount = obj->nVertices;
            if (m_vertexCount > 0)
            {
                m_vertices.resize(static_cast<size_t>(m_vertexCount));

                double *xptr = obj->padfX, *yptr = obj->padfY;
                for (int i = 0; i < m_vertexCount; ++i)
                {
                    m_vertices[i] = make_shared<Point>
                    pptr->Set(*xptr, *yptr);
                    ++xptr;
                    ++yptr;
                }
            }
            m_bounds.ymin = obj->dfYMin;
            m_bounds.xmin = obj->dfXMin;
            m_bounds.ymax = obj->dfYMax;
            m_bounds.xmax = obj->dfXMax;

            int *ps = obj->panPartStart;
            int *pt = obj->panPartType;
            Part part;
            m_parts.clear();
            for (int n = 0; n < obj->nParts; ++n)
            {
                part.type = *pt;
                part.offset = *ps;
                if ((n+1) == obj->nParts)
                {
                    part.length = m_vertexCount - *ps;
                }
                else
                {
                    part.length = *(ps+1) - *ps;
                }
                m_parts.push_back(part);
                ++ps;
                ++pt;
            }
        }
    }

    void ShapeObject::assign(const ShapeObject& other)
    {
        if (&other != this)
        {
            if (m_index >= 0)
            {
                Destroy();
            }
            m_index = other.m_index;
            m_type = other.m_type;
            m_vertexCount = other.m_vertexCount;
            if (m_vertexCount > 0)
            {
                m_vertices = new Point<double> [m_vertexCount];
                memcpy(m_vertices, other.m_vertices, other.m_vertexCount * sizeof(Point<double>));
            }
            m_bounds = other.m_bounds;
            m_parts = other.m_parts;
        }
    }

    bool ShapeObject::isValid() const
    {
        return m_index >= 0;
    }

    int ShapeObject::getIndex() const
    {
        return m_index;
    }

    int ShapeObject::getType() const
    {
        return m_type;
    }

    QString ShapeObject::getTypeString() const
    {
        return ShapeTypeAsString(m_type);
    }

    int ShapeObject::getVertexCount() const
    {
        return m_vertexCount;
    }

    const Point<double>* ShapeObject::getVertices() const
    {
        return const_cast<const Point<double>*>(m_vertices);
    }

    Box<double> ShapeObject::getBounds() const
    {
        return m_bounds;
    }

    std::vector<ShapeObject::Part> ShapeObject::getParts() const
    {
        return m_parts;
    }

    QString ShapeTypeAsString(int shapeType)
    {
        switch (shapeType)
        {
            case SHPT_NULL:
                return "None";
            case SHPT_POINT:
                return "2D Point";
            case SHPT_ARC:
                return "2D Arc";
            case SHPT_POLYGON:
                return "2D Polygon";
            case SHPT_MULTIPOINT:
                return "2D Multi-point";
            case SHPT_POINTZ:
                return "3D Point";
            case SHPT_ARCZ:
                return "3D Arc";
            case SHPT_POLYGONZ:
                return "3D Polygon";
            case SHPT_MULTIPOINTZ:
                return "3D Multi-point";
            case SHPT_POINTM:
                return "2D Measure Point";
            case SHPT_ARCM:
                return "2D Measure Arc";
            case SHPT_POLYGONM:
                return "2D Measure Polygon";
            case SHPT_MULTIPOINTM:
                return "2D Measure Multi-point";
            case SHPT_MULTIPATCH:
                return "Multi-patch";
            default:
                return "Unknown";
        }
    }

}
