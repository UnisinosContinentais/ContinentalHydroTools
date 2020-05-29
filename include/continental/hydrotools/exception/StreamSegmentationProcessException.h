/*
* Developed by UNISINOS
* author: Cristian Gabriel de Abreu Heylmann
* email: cristianheylmann@unisinos.br
* date: October, 2019
*/

#ifndef CONTINENTAL_HYDROTOOLS_EXCEPTION_STREAMDEFINITION_PROCESS_H
#define CONTINENTAL_HYDROTOOLS_EXCEPTION_STREAMDEFINITION_PROCESS_H

#include <cstddef>
#include <QString>
#include <iostream>
#include <exception>

namespace continental {
namespace hydrotools {
namespace exception {

class StreamSegmentationProcessException : virtual public std::runtime_error
{
public:
    /// Construtor
    explicit StreamSegmentationProcessException(std::string error);
    /// Desconstrutor
    virtual ~StreamSegmentationProcessException() noexcept = default;
};




}
}
}

#endif //CONTINENTAL_HYDROTOOLS_EXCEPTION_STREAMDEFINITION_PROCESS_H
