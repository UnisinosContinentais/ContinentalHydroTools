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

using namespace std;
namespace continental {
namespace hydrotools {
namespace exception {

class StreamSegmentationProcessException: public exception
{
  virtual const char* what() const throw()
  {
    return "O processo do Stream Segmentation não foi processado com sucesso!";
  }
} streamSegmentationProcessException;


}
}
}

#endif //CONTINENTAL_HYDROTOOLS_EXCEPTION_STREAMDEFINITION_PROCESS_H
