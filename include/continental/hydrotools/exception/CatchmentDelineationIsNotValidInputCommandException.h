/*
* Developed by UNISINOS
* author: Cristian Gabriel de Abreu Heylmann
* email: cristianheylmann@unisinos.br
* date: October, 2019
*/

#ifndef CONTINENTAL_HYDROTOOLS_EXCEPTION_CATCHMENTDELINEATION_IS_NOT_VALID_DATA_H
#define CONTINENTAL_HYDROTOOLS_EXCEPTION_CATCHMENTDELINEATION_IS_NOT_VALID_DATA_H

#include <cstddef>
#include <QString>
#include <iostream>
#include <exception>

using namespace std;

namespace continental {
namespace hydrotools {
namespace exception {

class CatchmentDelineationIsNotValidInputCommandException : virtual public std::runtime_error
{
public:
    /// Construtor
    explicit CatchmentDelineationIsNotValidInputCommandException();
    /// Desconstrutor
    virtual ~CatchmentDelineationIsNotValidInputCommandException() noexcept = default;
};

}
}
}

#endif // CONTINENTAL_HYDROTOOLS_EXCEPTION_CATCHMENTDELINEATION_IS_NOT_VALID_DATA_H
