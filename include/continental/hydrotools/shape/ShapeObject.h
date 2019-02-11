#ifndef IPHYDRORASTERTOOLS_SHAPE_SHAPEOBJECT_H
#define IPHYDRORASTERTOOLS_SHAPE_SHAPEOBJECT_H

#include <QString>
#include <vector>
#include <memory>
#include <vector>
#include <shapelib/shapefil.h>

#include "continental/hydrotools/shape/Box.h"
#include "continental/hydrotools/shape/Point.h"
#include "continental/hydrotools/shape/Part.h"

namespace IPHydroRasterTools
{
    namespace Shape {
        using namespace std;

        class ShapeObject
        {
            protected:
                int m_index;
                int m_type;
                size_t m_vertexCount;
                shared_ptr<vector<shared_ptr<Point<double>>>> m_vertices;
                Box<double> m_bounds;
                vector<Part> m_parts;

            public:
                ShapeObject();
                ShapeObject(const SHPObject* obj);
                ShapeObject(const ShapeObject& other);

                ShapeObject& operator = (const ShapeObject& other);

                void assign(const SHPObject* obj);

                bool isValid() const;

            protected:
                void destroy();
                void assign(const ShapeObject& obj);

            public:
                int getIndex() const;
                int getType() const;
                QString getTypeString() const;

                size_t getVertexCount() const;
                const shared_ptr<vector<shared_ptr<Point<double>>>> getVertices() const;
                Box<double> getBounds() const;

                std::vector<Part> getParts() const;
        };

        QString shapeTypeAsString(int shapetype);
    }
}

#endif // IPHYDRORASTERTOOLS_SHAPE_SHAPEOBJECT_H
