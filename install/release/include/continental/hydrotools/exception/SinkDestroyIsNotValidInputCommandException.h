/*
* Developed by UNISINOS
* author: Cristian Gabriel de Abreu Heylmann
* email: cristianheylmann@unisinos.br
* date: October, 2019
*/

#ifndef CONTINENTAL_HYDROTOOLS_EXCEPTION_SINKDESTROY_IS_NOT_VALID_DATA_H
#define CONTINENTAL_HYDROTOOLS_EXCEPTION_SINKDESTROY_IS_NOT_VALID_DATA_H

#include <cstddef>
#include <QString>
#include <iostream>
#include <exception>
using namespace std;
namespace continental {
namespace hydrotools {
namespace exception {


class SinkDestroyIsNotValidInputCommandException: public exception
{
  virtual const char* what() const throw()
  {
    return "Dados inválidos para realizar o processo do Sink Destroy!";
  }
} sinkDestroyIsNotValidInputCommandException;

}
}
}

#endif //CONTINENTAL_HYDROTOOLS_EXCEPTION_SINKDESTROY_IS_NOT_VALID_DATA_H
