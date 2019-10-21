#include "continental/hydrotools/util/HDF5Util.h"
#include <QFile>
#include <QRegularExpression>

namespace continental {
namespace hydrotools {
namespace util {

bool HDF5Util::pathExists(hid_t id, const QString &path)
{
    return H5Lexists(id, path.toStdString().c_str(), H5P_DEFAULT) > 0;
}

H5::Group HDF5Util::createOpenGroup(const H5::H5File &target, const QString &groupName)
{
    if (pathExists(target.getId(), groupName))
    {
        return target.openGroup(groupName.toStdString());
    }

    return target.createGroup(groupName.toStdString());
}

H5::Group HDF5Util::createOpenGroupRecursive(const H5::H5File &target, const QString &groupName)
{
    if (pathExists(target.getId(), groupName))
    {
        return target.openGroup(groupName.toStdString());
    }

    auto groups = groupName;
    QRegularExpression re("^/+");
    groups.replace(re, "");
    QRegularExpression re2("/+");
    groups.replace(re2, "/");

    auto parts = groups.split("/");

    QString currentGroup;
    H5::Group group;
    for (const auto &part : parts)
    {
        currentGroup += "/" + part;

        group = createOpenGroup(target, currentGroup);
    }

    return group;
}

H5::H5File HDF5Util::getFileOrGenerateIfNotExists(const QString &path)
{
    if(QFile::exists(path))
    {
        H5::H5File fileDAO(path.toLocal8Bit().constData(), H5F_ACC_RDWR);

        return fileDAO;
    }


    H5::H5File fileDAO(path.toLocal8Bit().constData(), H5F_ACC_EXCL);

    return fileDAO;
}

bool HDF5Util::hasDataSet(H5::Group &target, const QString &dataSetName)
{
    try {
        target.openDataSet(dataSetName.toStdString());
    }
    catch(H5::GroupIException not_found_error) {
        return false;
    }

    return true;
}

int HDF5Util::unlinkObject(H5::H5Object &parent, const QString &childName)
{
    return H5Ldelete(parent.getId(), childName.toStdString().data(), H5P_DEFAULT);
}

H5::DataSet HDF5Util::createOrUpdateDataset(H5::Group &group, const QString &name, const H5::DataType &dataType, const H5::DataSpace &dataSpace, const H5::DSetCreatPropList &createPlist)
{
    if (hasDataSet(group, name)) {
        unlinkObject(group, name);
    }

    return group.createDataSet(name.toStdString(), dataType, dataSpace, createPlist);
}

std::map<QString, QVariant> HDF5Util::readAttributes(QString pathFile, QString groupName, QString datasetName)
{
    std::map<QString, QVariant> result;

    try {
        H5::H5File hdf5File(pathFile.toLocal8Bit().constData(), H5F_ACC_RDWR);
        H5::Group group(hdf5File.openGroup(groupName.toLocal8Bit()));

        if (hasDataSet(group, datasetName)) {
            H5::DataSet dataset = group.openDataSet(datasetName.toStdString());
            result = readAttributes(dataset);
            dataset.close();
            group.close();
            hdf5File.close();
        }

    }
    catch (H5::FileIException ex ) {}
    catch (H5::GroupIException ex ) {}
    catch (H5::DataSetIException ex ) {}

    return result;
}

std::map<QString, QVariant> HDF5Util::readAttributes(H5::DataSet &dataset)
{
    std::map<QString, QVariant> result;

    auto limit = static_cast<size_t>(dataset.getNumAttrs());
    for(size_t i = 0; i < limit; ++i)
    {
        auto attribute = dataset.openAttribute(static_cast<const unsigned int>(i));
        switch (attribute.getDataType().getClass())
        {
        case H5T_INTEGER:
        {
            int attrData[1];
            attribute.read(H5::PredType::NATIVE_INT, attrData);
            result[QString::fromStdString(attribute.getName())] = QVariant(static_cast<int>(attrData[0]));
            break;
        }
        case H5T_FLOAT:
        {
            double attrData[1];
            attribute.read(H5::PredType::NATIVE_DOUBLE, attrData);
            result[QString::fromStdString(attribute.getName())] = QVariant(static_cast<double>(attrData[0]));
            break;
        }
        default:
            throw std::exception("HDF5Util::readAttributes - Can not type the attribute type.");
        }
    }

    return result;
}

void HDF5Util::writeAttributes(H5::DataSet &dataset, H5::DataSpace &attrDataspace, const std::map<QString, QVariant> &attributes)
{
    for (auto &pair : attributes)
    {
        const H5std_string attr(pair.first.toStdString());
        switch (static_cast<QMetaType::Type>(pair.second.type()))
        {
            case QMetaType::Type::Int:
            {
                const int attrData[1] = { pair.second.toInt() };
                H5::Attribute attribute = dataset.createAttribute(attr, H5::PredType::STD_I32BE, attrDataspace);
                attribute.write(H5::PredType::NATIVE_INT, attrData);
                break;
            }
            case QMetaType::Type::Double:
            {
                const double attrData[1] = { pair.second.toDouble() };
                H5::Attribute attribute = dataset.createAttribute(attr, H5::PredType::NATIVE_DOUBLE, attrDataspace);
                attribute.write(H5::PredType::NATIVE_DOUBLE, attrData);
                break;
            }
            default:
                QString text = "HDF5Util::writeAttributes - Can not type the attribute type ";
                text += pair.second.typeName();
                text += ".";
                throw std::exception(text.toLocal8Bit().constData());
        }
    }
}

} // namespace util
} // namespace hydrotools
} // namespace continental
