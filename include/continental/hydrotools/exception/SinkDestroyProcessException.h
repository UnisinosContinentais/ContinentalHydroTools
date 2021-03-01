/*
* Developed by UNISINOS
* author: Cristian Gabriel de Abreu Heylmann
* email: cristianheylmann@unisinos.br
* date: October, 2019
*/

#ifndef CONTINENTAL_HYDROTOOLS_EXCEPTION_SINKDESTROY_PROCESS_H
#define CONTINENTAL_HYDROTOOLS_EXCEPTION_SINKDESTROY_PROCESS_H

#include <stdexcept>

namespace continental {
namespace hydrotools {
namespace exception {

class SinkDestroyProcessException : virtual public std::runtime_error
{
public:
    /// Construtor
    explicit SinkDestroyProcessException();
    /// Desconstrutor
    virtual ~SinkDestroyProcessException() noexcept = default;
};

}
}
}

#endif //CONTINENTAL_HYDROTOOLS_EXCEPTION_SINKDESTROY_PROCESS_H
