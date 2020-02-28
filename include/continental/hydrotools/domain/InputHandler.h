/*
* Developed by UNISINOS
* author: Cristian Gabriel de Abreu Heylmann
* email: cristianheylmann@unisinos.br
* date: October, 2019
*/

#ifndef CONTINENTAL_HYDROTOOLS_DOMAIN_INPUT_HANDLER_H
#define CONTINENTAL_HYDROTOOLS_DOMAIN_INPUT_HANDLER_H

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
/// <summary>
/// Classe base da célula, para indicar a posição na matriz
/// </summary>
// invoker
#define SIZE 100
class InputHandler
{
public:
    /*~InputHandler();
	InputHandler() : m_command(new NullCommand), m_count(0), m_index(0) {}
	void SetCommand(Command *command) { m_command = command; }
	void DoCommand();
private:
	Command *m_command;
	Command *m_executed_commands[SIZE];   // InputHandler owns the command objects (and must delete them when it's done with them)
	int m_count;                          // total executed commands
    int m_index;                          // index one past last current command*/
};


}
}
}

#endif // CONTINENTAL_HYDROTOOLS_DOMAIN_INPUT_HANDLER_H
