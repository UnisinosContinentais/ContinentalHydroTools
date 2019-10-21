/*
* Developed by UNISINOS
* author: Cristian Gabriel de Abreu Heylmann
* email: cristianheylmann@unisinos.br
* date: October, 2019
*/

#ifndef CONTINENTAL_HYDROTOOLS_DOMAIN_WATERSHEDDELINEATION_COMMAND_INPUT_H
#define CONTINENTAL_HYDROTOOLS_DOMAIN_WATERSHEDDELINEATION_COMMAND_INPUT_H

#include <cstddef>
#include <QString>
#include <QString>
#include "AbstractCommandInput.h"
#include "FileCommand.h"

namespace continental {
namespace hydrotools {
namespace domain {

class WatershedDelineationCommandInput : AbstractCommandInput
{
public:
    /// Construtor
    WatershedDelineationCommandInput(QStringList argv);
    void prepare();

    void setFlowDirectionInput (const FileCommand flowDirectionInput);
    void setWatershedDelineationOutput (const FileCommand watershedDelineationOutput);

    FileCommand getFlowDirectionInput() const;
    FileCommand getWatershedDelineationOutput() const;

    size_t getRow() const;
    void setRow(const size_t &row);

    size_t getCol() const;
    void setCol(const size_t &col);

private:
    FileCommand m_flowDirectionInput;
    FileCommand m_watershedDelineationOutput;
    size_t m_row;
    size_t m_col;
};



}
}
}

#endif // CONTINENTAL_HYDROTOOLS_DOMAIN_WATERSHEDDELINEATION_COMMAND_INPUT_H
