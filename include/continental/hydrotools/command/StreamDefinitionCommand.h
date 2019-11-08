/*
* Developed by UNISINOS
* author: Cristian Gabriel de Abreu Heylmann
* email: cristianheylmann@unisinos.br
* date: October, 2019
*/

#ifndef CONTINENTAL_HYDROTOOLS_COMMAND_STREAMDEFINITION_COMMAND_H
#define CONTINENTAL_HYDROTOOLS_COMMAND_STREAMDEFINITION_COMMAND_H

#include <cstddef>
#include <QString>
#include "../domain/StreamDefinitionCommandInput.h"
#include "AbstractCommand.h"
namespace continental {
namespace hydrotools {

namespace domain {
class StreamDefinitionCommandInput;
}
namespace command
{
    class AbstractCommand;

    /** @brief Classe de implentação do StreamDefinitionCommand
     *  Esta classe é derivada de AbstractCommand
     */
    class StreamDefinitionCommand : public AbstractCommand
    {
        public:
            /// Construtor
            /// @param estrutura de dados com os parametros de entrada do console que serão utilizados no método prepare
            StreamDefinitionCommand(domain::StreamDefinitionCommandInput streamDefinitionCommandInput);

            /// Função que executa o comando em questão
            void execute();

            /// Destrutor
            ~StreamDefinitionCommand() = default;

        private:
            /// Atributo
            domain::StreamDefinitionCommandInput m_streamDefinitionCommandInput;
    };

}
}
}

#endif // CONTINENTAL_HYDROTOOLS_COMMAND_STREAMDEFINITION_COMMAND_H
