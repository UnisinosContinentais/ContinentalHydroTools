/*
* Developed by UNISINOS
* author: Luiz Felipe Bertoldi de Oliveira
* email: lbertoldio@unisinos.br
* date: January, 2019
*/

#ifndef CONTINENTAL_HYDROTOOLS_SHAPE_SHAPEFILE_H
#define CONTINENTAL_HYDROTOOLS_SHAPE_SHAPEFILE_H

#include <QString>
#include <shapefil.h>

#include "continental/hydrotools/service/shape/ShapeObject.h"

namespace continental
{
namespace hydrotools
{
namespace service
{
namespace shape
{

class ShapeFile
{
public:
class Bounds
{
    public:
    int x;
    int y;
    int z;
    int m;
};

protected:
SHPHandle m_handle;
int m_entityCount;
int m_shapeType;

public:
ShapeFile();
~ShapeFile();

int Open(const QString &filepath);
void Close();

int GetEntityCount()
{
    return m_entityCount;
}
int GetType() {
    return m_shapeType;
}
QString	GetTypeString();

int GetShape(size_t index, ShapeObject &object);
};

class ShapeFileError
{
};

}
}
}
}

#endif // CONTINENTAL_HYDROTOOLS_SHAPE_SHAPEFILE_H
