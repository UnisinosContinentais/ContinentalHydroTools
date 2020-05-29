/*
* Developed by UNISINOS
* author: Cristian Gabriel de Abreu Heylmann
* email: cristianheylmann@unisinos.br
* date: October, 2019
*/

#ifndef CONTINENTAL_HYDROTOOLS_EXCEPTION_WATERSHEDDELINEATION_PROCESS_H
#define CONTINENTAL_HYDROTOOLS_EXCEPTION_WATERSHEDDELINEATION_PROCESS_H

#include <cstddef>
#include <QString>
#include <iostream>
#include <exception>

namespace continental {
namespace hydrotools {
namespace exception {

class WatershedDelineationProcessException : virtual public std::runtime_error
{
public:
    /// Construtor
    explicit WatershedDelineationProcessException();
    /// Desconstrutor
    virtual ~WatershedDelineationProcessException() noexcept = default;
};


}
}
}

#endif //CONTINENTAL_HYDROTOOLS_EXCEPTION_WATERSHEDDELINEATION_PROCESS_H
