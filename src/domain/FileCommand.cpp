#include "continental/hydrotools/domain/FileCommand.h"

using namespace std;

namespace continental {
namespace hydrotools {
namespace domain {

FileCommand::FileCommand() { }

FileCommand::FileCommand(QString path, QString group)
{
    m_pathFile = path;
    m_groupName =  group;
}

void FileCommand::setPathFile(const QString pathFile)
{
    m_pathFile = pathFile;
}

QString FileCommand::getPathFile() const
{
    return m_pathFile;
}

void FileCommand::setGroupName(const QString groupName)
{
    m_groupName = groupName;
}

QString FileCommand::getGroupName() const
{
    return m_groupName;
}

}
}
}
