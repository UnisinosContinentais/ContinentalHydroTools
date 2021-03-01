/*
* Developed by UNISINOS
* author: Luiz Felipe Bertoldi de Oliveira
* email: lbertoldio@unisinos.br
* date: February, 2021
*/
#ifndef CONTINENTAL_HYDROTOOLS_EXCEPTION_FILENOTFOUNDEXCEPTION_H
#define CONTINENTAL_HYDROTOOLS_EXCEPTION_FILENOTFOUNDEXCEPTION_H

#include <stdexcept>

namespace continental {
namespace hydrotools {
namespace exception {

class FileNotFoundException : public std::runtime_error
{
public:
    /// Construtor
    explicit FileNotFoundException();
    /// Desconstrutor
    virtual ~FileNotFoundException() noexcept = default;
};

} // namespace exception
} // namespace hydrotools
} // namespace continental

#endif // CONTINENTAL_HYDROTOOLS_EXCEPTION_FILENOTFOUNDEXCEPTION_H
