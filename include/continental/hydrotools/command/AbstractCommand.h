/*
* Developed by UNISINOS
* author: Cristian Gabriel de Abreu Heylmann
* email: cristianheylmann@unisinos.br
* date: October, 2019
*/

#ifndef CONTINENTAL_HYDROTOOLS_COMMAND_ABSTRACTCOMMAND_H
#define CONTINENTAL_HYDROTOOLS_COMMAND_ABSTRACTCOMMAND_H

#include <cstddef>
#include <QString>

namespace continental {
namespace hydrotools {
namespace command {

class AbstractCommand
{
    public:
        virtual ~AbstractCommand();
        virtual void execute() = 0;
    protected:
        AbstractCommand();
};

}
}
}

#endif //CONTINENTAL_HYDROTOOLS_COMMAND_ABSTRACTCOMMAND_H
