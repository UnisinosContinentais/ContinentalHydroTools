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
        class WatershedDelineationCommand : public AbstractCommand
        {
            public:
                WatershedDelineationCommand(domain::WatershedDelineationCommandInput watershedDelineationCommandInput);
                void execute();
            private:
                domain::WatershedDelineationCommandInput m_watershedDelineationCommandInput;
        };

    }
}
}

#endif // CONTINENTAL_HYDROTOOLS_COMMAND_WATERSHEDDELINEATION_COMMAND_H
