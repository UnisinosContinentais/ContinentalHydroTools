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
/** @brief Classe Modelo de Comandos de Entrada
 *  Esta classe é utilizada como modelo para suas derivadas
 */
class AbstractCommandInput
{
public:
    /// Função virtual de implementação do Método de prepraração de argumentos
    virtual void prepare() = 0;

protected:
    /// Construtor
    AbstractCommandInput();

    /// Atributo
    QStringList m_argv;
};

}
}
}

#endif //CONTINENTAL_HYDROTOOLS_DOMAIN_ABSTRACTCOMMAND_INPUT_H
