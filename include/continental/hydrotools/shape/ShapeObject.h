#ifndef CONTINENTAL_HYDRO_TOOLS_SHAPE_SHAPEOBJECT_H
#define CONTINENTAL_HYDRO_TOOLS_SHAPE_SHAPEOBJECT_H

#include <QString>
#include <vector>
#include <memory>
#include <vector>
#include <shapelib/shapefil.h>

#include "continental/hydrotools/shape/Box.h"
#include "continental/hydrotools/shape/Point.h"
#include "continental/hydrotools/shape/Part.h"

namespace continental
{
namespace hydrotools
{
namespace shape
{

class ShapeObject
{
protected:
int m_index;
int m_type;
size_t m_vertexCount;
std::shared_ptr<std::vector<std::shared_ptr<Point<double>>>> m_vertices;
Box<double> m_bounds;
std::vector<Part> m_parts;

public:
ShapeObject();
ShapeObject(const SHPObject* obj);
ShapeObject(const ShapeObject& other);

ShapeObject& operator = (const ShapeObject& other);

void assign(const SHPObject* obj);

bool isValid() const;
int getIndex() const;
int getType() const;
QString getTypeString() const;

size_t getVertexCount() const;
const std::shared_ptr<std::vector<std::shared_ptr<Point<double>>>> getVertices() const;
Box<double> getBounds() const;

std::vector<Part> getParts() const;

protected:
void destroy();
void assign(const ShapeObject& obj);
};

QString shapeTypeAsString(int shapetype);

}
}
}

#endif // CONTINENTAL_HYDRO_TOOLS_SHAPE_SHAPEOBJECT_H
