/*
* Developed by UNISINOS
* author: Cristian Gabriel de Abreu Heylmann
* email: cristianheylmann@unisinos.br
* date: October, 2019
*/

#ifndef CONTINENTAL_HYDROTOOLS_DOMAIN_CATCHMENT_COMMAND_INPUT_H
#define CONTINENTAL_HYDROTOOLS_DOMAIN_CATCHMENT_COMMAND_INPUT_H

#include <cstddef>
#include <QString>

//*******************************************************************
//CLASSES ESPECÍFICAS DE CÉLULAS UTILIZADAS PELO PROGRAMA
//*******************************************************************

namespace continental
{
namespace hydrotools
{
namespace domain
{

class CatchmentCommandInput
{
public:
    /// Construtor
    CatchmentCommandInput();

    void setFlowDirectionInput (const FileCommandInput flowDirectionInput);
    FileCommandInput getFlowDirectionInput() const;

private:
    FileCommandInput m_flowDirectionInput;
};



}
}
}

#endif // CONTINENTAL_HYDROTOOLS_DOMAIN_CATCHMENT_COMMAND_INPUT_H
