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
namespace command {
    class AbstractCommand;
    class StreamSegmentationCommand : public AbstractCommand
    {
        public:
            StreamSegmentationCommand(domain::StreamSegmentationCommandInput streamSegmentationCommandInput);
            void execute() ;
        private:
            domain::StreamSegmentationCommandInput m_streamSegmentationCommandInput;
    };

}
}
}

#endif // CONTINENTAL_HYDROTOOLS_COMMAND_STREAMSEGMENTATION_COMMAND_H
