/*
* Developed by UNISINOS
* author: Cristian Gabriel de Abreu Heylmann
* email: cristianheylmann@unisinos.br
* date: October, 2019
*/

#ifndef CONTINENTAL_HYDROTOOLS_COMMAND_FLOWACCUMULATION_COMMAND_H
#define CONTINENTAL_HYDROTOOLS_COMMAND_FLOWACCUMULATION_COMMAND_H

#include <cstddef>
#include <QString>
#include "../domain/FlowAccumulationCommandInput.h"
#include "AbstractCommand.h"

namespace continental {
namespace hydrotools {

namespace domain {
    class FlowAccumulationCommandInput;
}
namespace command
{
    class AbstractCommand;
    /** @brief Classe de implentação do FlowAccumulationCommand
     *  Esta classe é derivada de AbstractCommand
     */
    class FlowAccumulationCommand : public AbstractCommand
    {
        public:
            /// Construtor
            /// @param estrutura de dados com os parametros de entrada do console que serão utilizados no método prepare
            FlowAccumulationCommand(domain::FlowAccumulationCommandInput flowAccumulationCommand);

            /// Função que executa o comando em questão
            void execute();

            /// Destrutor
            ~FlowAccumulationCommand() = default;

        private:
            /// Atributo
            domain::FlowAccumulationCommandInput m_flowAccumulationCommand;
    };
}
}
}

#endif // CONTINENTAL_HYDROTOOLS_COMMAND_FLOWACCUMULATION_COMMAND_H
