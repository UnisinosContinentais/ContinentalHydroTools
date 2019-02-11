#ifndef IPHYDRORASTERTOOLS_SHAPE_SHAPEFILE_H
#define IPHYDRORASTERTOOLS_SHAPE_SHAPEFILE_H

#include <QString>
#include <shapelib/shapefil.h>

#include "continental/hydrotools/shape/ShapeObject.h"

namespace IPHydroRasterTools
{
    namespace Shape
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

#endif // IPHYDRORASTERTOOLS_SHAPE_SHAPEFILE_H
