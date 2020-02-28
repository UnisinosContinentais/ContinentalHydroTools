/*
* Developed by UNISINOS
* author: Cristian Gabriel de Abreu Heylmann
* email: cristianheylmann@unisinos.br
* date: October, 2019
*/

#ifndef CONTINENTAL_HYDROTOOLS_COMMAND_SINKDESTROY_COMMAND_H
#define CONTINENTAL_HYDROTOOLS_COMMAND_SINKDESTROY_COMMAND_H

#include <cstddef>
#include <QString>
#include "../domain/HeuristicSinkRemovalCommandInput.h"
#include "AbstractCommand.h"
namespace continental {
namespace hydrotools {

namespace domain {
    class HeuristicSinkRemovalCommandInput;
}
namespace command
{
    class AbstractCommand;

    /** @brief Classe de implentação do SinkDestroyCommand
     *  Esta classe é derivada de AbstractCommand
     */
    class SinkDestroyCommand : public AbstractCommand
    {
        public:
            /// Construtor
            /// @param estrutura de dados com os parametros de entrada do console que serão utilizados no método prepare
            SinkDestroyCommand(domain::HeuristicSinkRemovalCommandInput sinkDestroyCommandInput);

            /// Função que executa o comando em questão
            void execute();

            /// Destrutor
            ~SinkDestroyCommand() = default;

        private:
            /// Atributo
            domain::HeuristicSinkRemovalCommandInput m_heuristicSinkRemovalCommandInput;
    };

}
}
}

#endif // CONTINENTAL_HYDROTOOLS_COMMAND_CATCHMENT_COMMAND_H
