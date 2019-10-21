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
        class CatchmentCommand : public AbstractCommand
        {
            public:
                CatchmentCommand(domain::CatchmentCommandInput catchmentCommandInput);
                void execute() ;
            private:
                domain::CatchmentCommandInput m_catchmentCommandInput;
        };

    }
}
}

#endif // CONTINENTAL_HYDROTOOLS_COMMAND_CATCHMENT_COMMAND_H
