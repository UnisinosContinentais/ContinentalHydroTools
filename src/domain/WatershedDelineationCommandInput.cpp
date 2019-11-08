#include "continental/hydrotools/domain/WatershedDelineationCommandInput.h"
#include "continental/hydrotools/exception/WatershedDelineationIsNotValidInputCommandException.h"
#include <iostream>
#include <exception>
#include "QStringList"

using namespace std;
using namespace continental::hydrotools::domain;
using namespace continental::hydrotools::exception;

namespace continental {
namespace hydrotools {
namespace domain {

WatershedDelineationCommandInput::WatershedDelineationCommandInput(QStringList argv)
{
    m_argv = argv;
}

/*
 * Modelo de entrada do Console
 * ===============================
 * [2] = pathFlowDirectionInput
 * [3] = groupFlowDirectionInput
 * [5] = pathCatchmentDelineationOutput
 * [6] = groupCatchmentDelineationOutput
 * [7] = row
 * [8] = col
*/
void WatershedDelineationCommandInput::prepare()
{
    if(m_argv.length() != 8)
    {
        throw exception::WatershedDelineationIsNotValidInputCommandException();
    }

    //Parse dos parametros de entradas do console
    auto fileFlowDirectionInput = FileCommand(m_argv[2], m_argv[3]);
    auto fileCatchmentDelineationOutput = FileCommand(m_argv[4], m_argv[5]);

    const size_t col = static_cast<size_t>(m_argv[6].toLongLong());
    const size_t row = static_cast<size_t>(m_argv[7].toLongLong());

    //prepara o objeto
    setFlowDirectionInput(fileFlowDirectionInput);
    setWatershedDelineationOutput(fileCatchmentDelineationOutput);
    setRow(row);
    setCol(col);
}

void WatershedDelineationCommandInput::setFlowDirectionInput(const FileCommand flowDirectionInput)
{
    m_flowDirectionInput = flowDirectionInput;
}

void WatershedDelineationCommandInput::setWatershedDelineationOutput(const FileCommand watershedDelineationOutput)
{
    m_watershedDelineationOutput = watershedDelineationOutput;
}

FileCommand WatershedDelineationCommandInput::getFlowDirectionInput() const
{
    return m_flowDirectionInput;
}

FileCommand WatershedDelineationCommandInput::getWatershedDelineationOutput() const
{
    return m_watershedDelineationOutput;
}

size_t WatershedDelineationCommandInput::getRow() const
{
    return m_row;
}

void WatershedDelineationCommandInput::setRow(const size_t &row)
{
    m_row = row;
}

size_t WatershedDelineationCommandInput::getCol() const
{
    return m_col;
}

void WatershedDelineationCommandInput::setCol(const size_t &col)
{
    m_col = col;
}

}
}
}




