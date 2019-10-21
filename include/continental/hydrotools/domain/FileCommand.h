/*
* Developed by UNISINOS
* author: Cristian Gabriel de Abreu Heylmann
* email: cristianheylmann@unisinos.br
* date: October, 2019
*/

#ifndef CONTINENTAL_HYDROTOOLS_sDOMAIN_IFILE_H
#define CONTINENTAL_HYDROTOOLS_sDOMAIN_IFILE_H

#include <cstddef>
#include <QString>

//*******************************************************************
//CLASSES ESPECÍFICAS DE CÉLULAS UTILIZADAS PELO PROGRAMA
//*******************************************************************

namespace continental {
namespace hydrotools {
namespace domain {

class FileCommand
{
public:
    FileCommand();

    FileCommand(QString path, QString group);

    void setPathFile (const QString name);

    QString getPathFile() const;

    void setGroupName (const QString name);

    QString getGroupName() const;
protected:
    QString m_pathFile;
    QString m_groupName;
};


}
}
}

#endif // CONTINENTAL_HYDROTOOLS_CELLS_H
