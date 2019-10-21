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
namespace command {
    class AbstractCommand;

    class SinkDestroyCommand : public AbstractCommand
    {
        public:
            SinkDestroyCommand(domain::HeuristicSinkRemovalCommandInput sinkDestroyCommandInput);
            void execute();
        private:
            domain::HeuristicSinkRemovalCommandInput m_heuristicSinkRemovalCommandInput;
    };

}
}
}

#endif // CONTINENTAL_HYDROTOOLS_COMMAND_CATCHMENT_COMMAND_H
