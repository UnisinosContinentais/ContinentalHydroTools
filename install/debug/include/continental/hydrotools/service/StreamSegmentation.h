/*
* Developed by UNISINOS
* author: Luiz Felipe Bertoldi de Oliveira
* email: lbertoldio@unisinos.br
* date: January, 2019
*/

#ifndef CONTINENTAL_HYDROTOOLS_STREAMSEGMENTATION_H
#define CONTINENTAL_HYDROTOOLS_STREAMSEGMENTATION_H

#include <QString>
#include <memory>
#include <continental/datamanagement/Raster.h>

//*******************************************************************
//DETERMINAÇÃO DO STREAM SEGMENTATION
//*******************************************************************

namespace continental
{
namespace hydrotools
{
namespace service
{

/// <summary>
/// Classe que define os trechos de rio
/// </summary>
class StreamSegmentation
{
private:
std::shared_ptr<datamanagement::Raster<short>> m_strDef;
std::shared_ptr<datamanagement::Raster<short>> m_flowDirection;

protected:
std::shared_ptr<datamanagement::Raster<short>> m_strSeg;

public:
StreamSegmentation();
virtual ~StreamSegmentation() = default;

void setFlowDirection(std::shared_ptr<datamanagement::Raster<short>> flowDirection);

void setStreamDefinition(std::shared_ptr<datamanagement::Raster<short>> streamDefinition);

/// <summary>
/// Separa os trechos de rio em todas as confluências
/// </summary>
virtual void segmentStreams();

std::shared_ptr<datamanagement::Raster<short>> getStreamSegmentation();

private:
//Verifica se algum vizinho de mesma cota, ou inferior já possui flow direction, atribuindo a mesma em caso verdadeiro
bool verifyStreamOutlet(int yc, int xc);

//Atualiza o atributo do stream definition
void updateSegments(int row, int column, int rowLast, int columnLast, short attribute);
};

}
}
}

#endif // CONTINENTAL_HYDROTOOLS_STREAMSEGMENTATION_H
