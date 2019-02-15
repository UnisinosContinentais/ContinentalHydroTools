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

}
}
}
