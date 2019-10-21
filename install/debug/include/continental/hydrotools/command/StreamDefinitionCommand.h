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
namespace command {
    class AbstractCommand;

    class StreamDefinitionCommand : public AbstractCommand
    {
        public:
            StreamDefinitionCommand(domain::StreamDefinitionCommandInput streamDefinitionCommandInput);
            void execute() ;
        private:
            domain::StreamDefinitionCommandInput m_streamDefinitionCommandInput;
    };

}
}
}

#endif // CONTINENTAL_HYDROTOOLS_COMMAND_STREAMDEFINITION_COMMAND_H
