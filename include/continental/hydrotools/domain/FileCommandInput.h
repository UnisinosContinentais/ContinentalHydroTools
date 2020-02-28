/*
* Developed by UNISINOS
* author: Cristian Gabriel de Abreu Heylmann
* email: cristianheylmann@unisinos.br
* date: October, 2019
*/
#ifndef CONTINENTAL_HYDROTOOLS_DOMAIN_IFILE_COMMAND_INPUT_H
#define CONTINENTAL_HYDROTOOLS_DOMAIN_IFILE_COMMAND_INPUT_H

#include <cstddef>
#include <QString>
#include "IFile.h"

//*******************************************************************
//CLASSES ESPECÍFICAS DE CÉLULAS UTILIZADAS PELO PROGRAMA
//*******************************************************************

namespace continental
{
namespace hydrotools
{
namespace domain
{
	
class IFile;
/// <summary>
/// Classe base da célula, para indicar a posição na matriz
/// </summary>
class FileCommandInput : IFile
{
public:
    /// Construtor
    FileCommandInput();
    FileCommandInput(QString path, QString group);
};

}
}
}

#endif //CONTINENTAL_HYDROTOOLS_DOMAIN_IFILE_COMMAND_INPUT_H
