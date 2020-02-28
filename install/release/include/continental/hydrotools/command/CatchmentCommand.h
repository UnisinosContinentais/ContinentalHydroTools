/*
* Developed by UNISINOS
* author: Cristian Gabriel de Abreu Heylmann
* email: cristianheylmann@unisinos.br
* date: October, 2019
*/

#ifndef CONTINENTAL_HYDROTOOLS_COMMAND_CATCHMENT_COMMAND_H
#define CONTINENTAL_HYDROTOOLS_COMMAND_CATCHMENT_COMMAND_H

#include <cstddef>
#include <QString>
#include "../domain/CatchmentCommandInput.h"
#include "AbstractCommand.h"

namespace continental {
namespace hydrotools
{
    namespace domain {
        class CatchmentCommandInput;
    }
    namespace command
    {
        /** @brief Classe de implentação do CatchmentCommand
         *  Esta classe é derivada de AbstractCommand
         */
        class CatchmentCommand : public AbstractCommand
        {
            public:
                /// Construtor
                /// @param estrutura de dados com os parametros de entrada do console que serão utilizados no método prepare
                CatchmentCommand(domain::CatchmentCommandInput catchmentCommandInput);

                /// Função que executa o comando em questão
                void execute();

                /// Destrutor
                ~CatchmentCommand() = default;

            private:
                /// Atributo
                domain::CatchmentCommandInput m_catchmentCommandInput;
        };

    }
}
}

#endif // CONTINENTAL_HYDROTOOLS_COMMAND_CATCHMENT_COMMAND_H
