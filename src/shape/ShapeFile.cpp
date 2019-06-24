#include <QString>
#include "continental/hydrotools/shape/ShapeFile.h"

namespace continental
{
namespace hydrotools
{
namespace shape
{

ShapeFile::ShapeFile() : m_handle(nullptr), m_entityCount(0), m_shapeType(0)
{
}

ShapeFile::~ShapeFile()
{
    Close();
}

int ShapeFile::Open(const QString &filepath)
{
    Close();
    m_handle = SHPOpen(filepath.toStdString().c_str(), "rb");
    if (m_handle == nullptr)
    {
        return -1;
    }
    else
    {
        SHPGetInfo(m_handle, &m_entityCount, &m_shapeType, nullptr, nullptr);
    }
    return 0;
}

void ShapeFile::Close()
{
    if (m_handle != nullptr)
    {
        SHPClose(m_handle);
    }
    m_handle = nullptr;
}

QString ShapeFile::GetTypeString()
{
    return shapeTypeAsString(m_shapeType);
}

int ShapeFile::GetShape(size_t index, ShapeObject &object)
{
    if (m_handle == nullptr)
    {
        return -1;
    }
    SHPObject *obj = SHPReadObject(m_handle, static_cast<int>(index));
    object.assign(obj);
    SHPDestroyObject(obj);

    return 0;
}

QString shapeTypeAsString(int shapeType)
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
}
}
