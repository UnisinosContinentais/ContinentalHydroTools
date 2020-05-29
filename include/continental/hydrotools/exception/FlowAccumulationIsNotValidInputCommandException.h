/*
* Developed by UNISINOS
* author: Cristian Gabriel de Abreu Heylmann
* email: cristianheylmann@unisinos.br
* date: October, 2019
*/

#ifndef CONTINENTAL_HYDROTOOLS_EXCEPTION_FLOWACCUMULATION_IS_NOT_VALID_DATA_H
#define CONTINENTAL_HYDROTOOLS_EXCEPTION_FLOWACCUMULATION_IS_NOT_VALID_DATA_H

#include <cstddef>
#include <QString>
#include <iostream>
#include <exception>

namespace continental {
namespace hydrotools {
namespace exception {

class FlowAccumulationIsNotValidInputCommandException : virtual public std::runtime_error
{
public:
    /// Construtor
    explicit FlowAccumulationIsNotValidInputCommandException();
    /// Desconstrutor
    virtual ~FlowAccumulationIsNotValidInputCommandException() noexcept = default;
};

}
}
}

#endif // CONTINENTAL_HYDROTOOLS_EXCEPTION_FLOWACCUMULATION_IS_NOT_VALID_DATA_H
