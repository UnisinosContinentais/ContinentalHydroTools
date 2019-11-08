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

namespace continental {
namespace hydrotools {
namespace domain {
/** @brief Classe de Comandos em Arquivos
 *  Esta classe tem as informações para persistência dos dados em arquivos físicos
 */
class FileCommand
{
public:
    /// Construtor
    FileCommand();

    /// Construtor
    /// @param path valor para local disisco do arquivo
    /// @param group valor para o Grupo em que sera gravado o raster/dataset
    FileCommand(QString path, QString group);

    /// Função para atualizar o valor do local do arquivo
    /// @param pathFile Valor do local do arquivo.
    void setPathFile (const QString pathFile);

    /// Função Getter da área do local do arquivo
    /// @return Retorna string do local do arquivo
    QString getPathFile() const;

    /// Função para atualizar o valor do Grupo do HDF5
    /// @param groupName Valor do Grupo do arquivo HDF5 onde as informações serão atualizadas/gravadas.
    void setGroupName (const QString groupName);

    /// Função Getter da área do Grupo do HDF5
    /// @return Retorna string do Grupo do HDF5
    QString getGroupName() const;

    /// Destrutor
    ~FileCommand() = default;

protected:
    /// Atributo
    QString m_pathFile;

    /// Atributo
    QString m_groupName;
};


}
}
}

#endif // CONTINENTAL_HYDROTOOLS_CELLS_H
