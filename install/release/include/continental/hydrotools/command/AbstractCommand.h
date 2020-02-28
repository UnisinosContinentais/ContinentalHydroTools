/*
* Developed by UNISINOS
* author: Cristian Gabriel de Abreu Heylmann
* email: cristianheylmann@unisinos.br
* date: October, 2019
*/
#ifndef CONTINENTAL_HYDROTOOLS_COMMAND_ABSTRACTCOMMAND_H
#define CONTINENTAL_HYDROTOOLS_COMMAND_ABSTRACTCOMMAND_H

#include <cstddef>
#include <QString>

namespace continental {
namespace hydrotools {
namespace command {

/** @brief Classe  modelo de Commandos
 * Esta classe define o modelo do padrão command de execução
 */
class AbstractCommand
{
    public:
        /// Destrutor
        virtual ~AbstractCommand();

        /// Função virtual de implementação do Método de Execução do comando
        virtual void execute() = 0;

    protected:
        /// Construtor
        AbstractCommand();
};

}
}
}

#endif //CONTINENTAL_HYDROTOOLS_COMMAND_ABSTRACTCOMMAND_H
