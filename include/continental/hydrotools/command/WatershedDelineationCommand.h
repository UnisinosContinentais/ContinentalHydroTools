/*
* Developed by UNISINOS
* author: Cristian Gabriel de Abreu Heylmann
* email: cristianheylmann@unisinos.br
* date: October, 2019
*/

#ifndef CONTINENTAL_HYDROTOOLS_COMMAND_WATERSHEDDELINEATION_COMMAND_H
#define CONTINENTAL_HYDROTOOLS_COMMAND_WATERSHEDDELINEATION_COMMAND_H

#include <cstddef>
#include <QString>
#include "../domain/WatershedDelineationCommandInput.h"
#include "AbstractCommand.h"
namespace continental {
namespace hydrotools
{
    namespace domain {
        class WatershedDelineationCommandInput;
    }
    namespace command
    {
        /** @brief Classe de implentação do Método de delineação da bacia Hidrográfica
         *  Esta classe é derivada de AbstractCommand
         */
        class WatershedDelineationCommand : public AbstractCommand
        {
            public:
                /// Construtor
                /// @param estrutura de dados com os parametros de entrada do console que serão utilizados no método prepare
                WatershedDelineationCommand(domain::WatershedDelineationCommandInput watershedDelineationCommandInput);

                /// Função que executa o comando em questão
                void execute();

                /// Destrutor
                ~WatershedDelineationCommand() = default;
            private:
                domain::WatershedDelineationCommandInput m_watershedDelineationCommandInput;
        };
    }
}
}

#endif // CONTINENTAL_HYDROTOOLS_COMMAND_WATERSHEDDELINEATION_COMMAND_H
