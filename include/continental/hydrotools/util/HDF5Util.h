/*
* Developed by UNISINOS - Projeto Continentais and Petrobras
* author: Luiz Felipe Bertoldi de Oliveira
* email: lbertoldio@unisinos.br
* date: June, 2019
*/
#ifndef CONTINENTAL_HYDROTOOLS_UTIL_HDF5UTIL_H
#define CONTINENTAL_HYDROTOOLS_UTIL_HDF5UTIL_H

#include <H5Cpp.h>
#include <H5Exception.h>
#include <map>
#include <string>
#include <QString>
#include <QVariant>


namespace continental {
namespace hydrotools {
namespace util {

class  HDF5Util {
public:
static bool pathExists(hid_t id, const QString& path);

static H5::Group createOpenGroup(const H5::H5File &target, const QString &groupName);

static H5::Group createOpenGroupRecursive(const H5::H5File &target, const QString &groupName);

static H5::H5File getFileOrGenerateIfNotExists(const QString& path);

static bool hasDataSet(H5::Group &target, const QString &dataSetName);

static int unlinkObject(H5::H5Object &target, const QString &dataSetName);

static H5::DataSet createOrUpdateDataset(H5::Group &group, const QString& name, const H5::DataType& dataType, const H5::DataSpace& dataSpace, const H5::DSetCreatPropList& createPlist = H5::DSetCreatPropList::DEFAULT);

static std::map<QString, QVariant> readAttributes(QString pathFile, QString groupName, QString datasetName);

static std::map<QString, QVariant> readAttributes(H5::DataSet &dataset);

static void writeAttributes(H5::DataSet &dataset, H5::DataSpace &attrDataspace, const std::map<QString, QVariant> &attributes);

private:
HDF5Util() = default;
};


} // namespace util
} // namespace hydrotools
} // namespace continental


#endif //CONTINENTAL_UTIL_HDF5UTIL_H
