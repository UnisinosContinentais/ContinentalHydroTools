/*
* Developed by UNISINOS
* author: Cristian Gabriel de Abreu Heylmann
* email: cristianheylmann@unisinos.br
* date: October, 2019
*/

#ifndef CONTINENTAL_HYDROTOOLS_DOMAIN_ABSTRACTCOMMAND_INPUT_H
#define CONTINENTAL_HYDROTOOLS_DOMAIN_ABSTRACTCOMMAND_INPUT_H

#include <cstddef>
#include <QString>
#include <QStringList>

namespace continental {
namespace hydrotools {
namespace domain {
/// <summary>
/// Classe base de parametros para Command
/// </summary>
class AbstractCommandInput
{
public:
    virtual void prepare() = 0;
protected:
    AbstractCommandInput();
    QStringList m_argv;
};


}
}
}

#endif //CONTINENTAL_HYDROTOOLS_DOMAIN_ABSTRACTCOMMAND_INPUT_H
