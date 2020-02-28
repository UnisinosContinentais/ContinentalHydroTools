/*
* Developed by UNISINOS
* author: Cristian Gabriel de Abreu Heylmann
* email: cristianheylmann@unisinos.br
* date: October, 2019
*/

#ifndef CONTINENTAL_HYDROTOOLS_EXCEPTION_STREAMDEFINITION_IS_NOT_VALID_DATA_H
#define CONTINENTAL_HYDROTOOLS_EXCEPTION_STREAMDEFINITION_IS_NOT_VALID_DATA_H

#include <cstddef>
#include <QString>
#include <iostream>
#include <exception>

using namespace std;

namespace continental {
namespace hydrotools {
namespace exception {

class StreamDefinitionIsNotValidInputCommandException : virtual public std::runtime_error
{
public:
    /// Construtor
    explicit StreamDefinitionIsNotValidInputCommandException();
    /// Desconstrutor
    virtual ~StreamDefinitionIsNotValidInputCommandException() noexcept = default;
};





}
}
}

#endif //CONTINENTAL_HYDROTOOLS_EXCEPTION_STREAMDEFINITION_IS_NOT_VALID_DATA_H
