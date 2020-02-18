/*
* Developed by UNISINOS
* author: Cristian Gabriel de Abreu Heylmann
* email: cristianheylmann@unisinos.br
* date: October, 2019
*/

#ifndef CONTINENTAL_HYDROTOOLS_COMMAND_ABSTRACT_RASTER_H
#define CONTINENTAL_HYDROTOOLS_COMMAND_ABSTRACT_RASTER_H

#include <cstddef>
#include <QString>
#include <H5Cpp.h>
#include <QString>
#include <QFile>
#include <QVariant>
#include <exception>
#include <memory>
#include <map>
#include "../util/HDF5Util.h"
#include "continental/datamanagement/Raster.h"
#include "../domain/FileCommand.h"

using namespace continental::hydrotools::util;
using namespace continental::hydrotools::domain;
using namespace continental::datamanagement;
using namespace std;

namespace continental {
namespace hydrotools {
namespace infrastructure {

template<class RasterType>
class AbstractRasterIO
{
public:
    /// Construtor
    AbstractRasterIO(FileCommand file)
    {
        setPathFile(file.getPathFile());
        setGroupName("FluvialSystem");
        setDatasetName(file.getGroupName());
    }

    void setGroupName(QString groupName) {
        m_groupName = groupName;
    }

    QString getGroupName() const
    {
        return m_groupName;
    }

    void setDatasetName(const QString &datasetName)
    {
        m_datasetName = datasetName;
    }

    QString getDatasetName() const
    {
        return m_datasetName;
    }

    QString getPathFile() const
    {
        return m_pathFile;
    }
    void setPathFile(QString pathFile){
        m_pathFile = pathFile;
    }

    bool existRasterIO(const QString &hdf5FileName, const QString &nameGroupRaster) const
    {
        bool result = false;
        try
        {
            // Open an existing file and dataset.
            if(QFile::exists(hdf5FileName.toLocal8Bit().constData()))
            {
                H5::H5File hdf5File(hdf5FileName.toLocal8Bit().constData(), H5F_ACC_RDWR);
                H5::Group group(hdf5File.openGroup(getGroupName().toLocal8Bit().constData()));
                H5::DataSet dataset = group.openDataSet(nameGroupRaster.toStdString());
                group.close();
                hdf5File.close();
                result = true;
            }
        }
        catch (H5::FileIException ex ) {}
        catch (H5::GroupIException ex ) {}
        catch (H5::DataSetIException ex ) {}
        catch (std::exception &e)
        {
            cout << e.what() << endl;
        }
        catch (...)
        {
            cout << "erro!" << endl;
        }
        return result;
    }


       /// Função de escrita do raster no dataset do HDF5.
       /// @param raster Raster a ser salvo no dataset.
       /// @param name Nome do raster.
       /// @param group Grupo no qual deva ser salvo o raster.
       void writeRasterIO(Raster<RasterType> &raster, const QString name,
                  unsigned short version, const map<QString, QVariant> &extraAttributes = map<QString, QVariant>()) const
       {
           auto fileDao = HDF5Util::getFileOrGenerateIfNotExists(getPathFile());
           auto group = HDF5Util::createOpenGroup(fileDao, getGroupName());

           // Create the data space for the dataset.
           const int RANK = 2;

           // Salva o nome do dataset
           const H5std_string datasetName(name.toStdString());
           // Dataset dimensions
           hsize_t dims[2];
           dims[0] = raster.getRows();
           dims[1] = raster.getCols();
           H5::DataSpace dataspace(RANK, dims);
           // Create the dataset.
           H5::DataSet dataset = HDF5Util::createOrUpdateDataset(group, name, std::is_floating_point<RasterType>::value ? H5::PredType::NATIVE_FLOAT : H5::PredType::STD_I32BE, dataspace);
           hsize_t dims2[1] = { 1 };
           // Create the data space for the attribute.
           H5::DataSpace attrDataspace = H5::DataSpace (1, dims2 );

           map<QString, QVariant> attributes;

           attributes["cols"] = QVariant(static_cast<int>(raster.getCols()));
           attributes["rows"] = QVariant(static_cast<int>(raster.getRows()));
           attributes["xOrigin"] = QVariant(static_cast<double>(raster.getXOrigin()));
           attributes["yOrigin"] = QVariant(static_cast<double>(raster.getYOrigin()));
           attributes["cellSize"] = QVariant(static_cast<double>(raster.getCellSize()));
           attributes["noDataValue"] = QVariant(static_cast<int>(raster.getNoDataValue()));
           attributes["version"] = version;

           for (auto pair : extraAttributes)
           {
               attributes[pair.first] = pair.second;
           }

           HDF5Util::writeAttributes(dataset, attrDataspace, attributes);

           // Data initialization.
           auto data = new RasterType[raster.getTotalCells()];
           for (size_t i = 0; i < raster.getTotalCells(); ++i)
           {
               data[i] = raster.getData(i);
           }
           // Write the data to the dataset using default memory space, file
           // space, and transfer properties.
           dataset.write(data, std::is_floating_point<RasterType>::value ? H5::PredType::NATIVE_FLOAT : H5::PredType::NATIVE_SHORT);
           dataset.close();

           delete[] data;

           group.close();
           fileDao.close();
       }

       ///Função de leitura do arquivo HDF5.
       /// @param hdf5FileName Caminho do arquivo a ser lido.
       /// @param nameGroupRaster Nome do grupo no dataset.
       /// @return retorna um Raster apartir do HDF5.
       std::pair<std::map<QString, QVariant>, Raster<RasterType>> readRasterIO(const QString &hdf5FileName, const QString &nameGroupRaster) const
       {
           std::pair<std::map<QString, QVariant>, Raster<RasterType>> result;
           try{
               // Open an existing file and dataset.
               if(QFile::exists(hdf5FileName.toLocal8Bit().constData()))
               {
                   H5::H5File hdf5File(hdf5FileName.toLocal8Bit().constData(), H5F_ACC_RDWR);
                   H5::Group group(hdf5File.openGroup(getGroupName().toLocal8Bit()));

                   H5::DataSet dataset = group.openDataSet(nameGroupRaster.toStdString());

                   result.first = HDF5Util::readAttributes(dataset);

                   size_t rows = static_cast<size_t>(result.first["rows"].toInt());
                   result.first.erase("rows");
                   size_t cols = static_cast<size_t>(result.first["cols"].toInt());
                   result.first.erase("cols");
                   double xOrigin = static_cast<double>(result.first["xOrigin"].toDouble());
                   result.first.erase("xOrigin");
                   double yOrigin = static_cast<double>(result.first["yOrigin"].toDouble());
                   result.first.erase("yOrigin");
                   double cellSize = static_cast<double>(result.first["cellSize"].toDouble());
                   result.first.erase("cellSize");
                   int noData = static_cast<int>(result.first["noDataValue"].toInt());
                   result.first.erase("noDataValue");

                   auto data = new RasterType[rows * cols];

                   dataset.read(data, std::is_floating_point<RasterType>::value ? H5::PredType::NATIVE_FLOAT : H5::PredType::NATIVE_SHORT);
                   Raster<RasterType> raster(rows, cols, xOrigin, yOrigin, cellSize, noData);
                   for (size_t i = 0; i < raster.getTotalCells(); ++i)
                   {
                      raster.setData(i, data[i]);
                   }

                   result.second = std::move(raster);

                   delete[] data;

                   group.close();
                   hdf5File.close();
               }
           }
           catch (H5::FileIException ex ) {}
           catch (H5::GroupIException ex ) {}
           catch (H5::DataSetIException ex ) {}
           return result;
       }
    private:
        QString m_pathFile;
        QString m_groupName;
        QString m_datasetName;
};

} //infrastructure
} //hydrotools
} //continental
#endif // CONTINENTAL_HYDROTOOLS_COMMAND_ABSTRACT_RASTER_H
