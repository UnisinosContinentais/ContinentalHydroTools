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
namespace command {
    class AbstractCommand;
    class FlowAccumulationCommand : public AbstractCommand
    {
        public:
            FlowAccumulationCommand(domain::FlowAccumulationCommandInput flowAccumulationCommand);
            void execute() ;
        private:
            domain::FlowAccumulationCommandInput m_flowAccumulationCommand;
    };
}
}
}

#endif // CONTINENTAL_HYDROTOOLS_COMMAND_FLOWACCUMULATION_COMMAND_H
