#include <memory>
#include <QString>

#include "continental/hydrotools/shape/ShapeObject.h"
#include "continental/hydrotools/shape/Part.h"

using namespace std;

namespace continental
{
namespace hydrotools
{
namespace shape
{

ShapeObject::ShapeObject() : m_index(-1), m_type(0), m_vertexCount(0)
{
}

ShapeObject::ShapeObject(const SHPObject* obj) : m_index(-1), m_type(0), m_vertexCount(0)
{
    assign(obj);
}

ShapeObject::ShapeObject(const ShapeObject &other) : m_index(-1), m_type(0), m_vertexCount(0)
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
    if (obj != nullptr) {
        if (m_index >= 0) {
            destroy();
        }
        m_index = obj->nShapeId;
        m_type = obj->nSHPType;
        m_vertexCount = static_cast<size_t>(obj->nVertices);
        if (m_vertexCount > 0)
        {
            m_vertices->resize(m_vertexCount);

            double *xptr = obj->padfX, *yptr = obj->padfY;
            for (size_t i = 0; i < m_vertexCount; ++i)
            {
                (*m_vertices.get())[i] = std::make_shared<Point<double>>(*xptr, *yptr);
                ++xptr;
                ++yptr;
            }
        }
        m_bounds.xMin(obj->dfXMin);
        m_bounds.yMin(obj->dfYMin);
        m_bounds.yMax(obj->dfYMax);
        m_bounds.xMax(obj->dfXMax);

        int *ps = obj->panPartStart;
        int *pt = obj->panPartType;
        Part part;

        m_parts.clear();
        auto nParts = static_cast<size_t>(obj->nParts);
        for (size_t n = 0; n < nParts; ++n)
        {
            part.type = *pt;
            part.offset = *ps;
            if ((n+1) == nParts)
            {
                part.length = m_vertexCount - static_cast<size_t>(*ps);
            }
            else
            {
                part.length = static_cast<size_t>(*(ps + 1) - *ps);
            }
            m_parts[n] = part;
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
            destroy();
        }
        m_index = other.m_index;
        m_type = other.m_type;
        m_vertexCount = other.m_vertexCount;
        if (m_vertexCount > 0)
        {
            m_vertices->clear();
            m_vertices->resize(other.getVertices()->size());
            size_t i = 0;
            for (const auto &vertice : *other.getVertices().get())
            {
                (*m_vertices.get())[i] = make_shared<Point<double>>(vertice->x(), vertice->y());
                ++i;
            }
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
    switch (m_type)
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

size_t ShapeObject::getVertexCount() const
{
    return m_vertexCount;
}

const shared_ptr<vector<shared_ptr<Point<double>>>> ShapeObject::getVertices() const
{
    return m_vertices;
}

Box<double> ShapeObject::getBounds() const
{
    return m_bounds;
}

std::vector<Part> ShapeObject::getParts() const
{
    return m_parts;
}

}
}
}
