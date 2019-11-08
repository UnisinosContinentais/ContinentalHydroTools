/*
* Developed by UNISINOS
* author: Cristian Gabriel de Abreu Heylmann
* email: cristianheylmann@unisinos.br
* date: October, 2019
*/

#ifndef CONTINENTAL_HYDROTOOLS_COMMAND_STREAMSEGMENTATION_COMMAND_H
#define CONTINENTAL_HYDROTOOLS_COMMAND_STREAMSEGMENTATION_COMMAND_H

#include <cstddef>
#include <QString>
#include "../domain/StreamSegmentationCommandInput.h"
#include "AbstractCommand.h"

namespace continental {
namespace hydrotools {
namespace domain {
    class StreamSegmentationCommandInput;
}
namespace command
{

    class AbstractCommand;
    /** @brief Classe de implentação do StreamSegmentationCommand
     *  Esta classe é derivada de AbstractCommand
     */
    class StreamSegmentationCommand : public AbstractCommand
    {
        public:
            /// Construtor
            /// @param estrutura de dados com os parametros de entrada do console que serão utilizados no método prepare
            StreamSegmentationCommand(domain::StreamSegmentationCommandInput streamSegmentationCommandInput);

            /// Função que executa o comando em questão
            void execute();

            /// Destrutor
            ~StreamSegmentationCommand() = default;
        private:
            domain::StreamSegmentationCommandInput m_streamSegmentationCommandInput;
    };
}
}
}

#endif // CONTINENTAL_HYDROTOOLS_COMMAND_STREAMSEGMENTATION_COMMAND_H
