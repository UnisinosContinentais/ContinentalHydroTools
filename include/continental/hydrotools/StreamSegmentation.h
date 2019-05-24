#ifndef CONTINENTAL_HYDRO_TOOLS_STREAMSEGMENTATION_H
#define CONTINENTAL_HYDRO_TOOLS_STREAMSEGMENTATION_H

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
/// <summary>
/// Classe que define os trechos de rio
/// </summary>
class StreamSegmentation
{
private:
std::shared_ptr<datamanagement::Raster<short>> m_strDef;
std::shared_ptr<datamanagement::Raster<short>> m_strSeg;
std::shared_ptr<datamanagement::Raster<short>> m_flowDirection;

public:
StreamSegmentation();

void setFlowDirection(std::shared_ptr<datamanagement::Raster<short>> flowDirection);

void setStreamDefinition(std::shared_ptr<datamanagement::Raster<short>> streamDefinition);

/// <summary>
/// Separa os trechos de rio em todas as confluências
/// </summary>
void segmentStreams();

std::shared_ptr<datamanagement::Raster<short>> getStreamSegmentation();

private:
//Verifica se algum vizinho de mesma cota, ou inferior já possui flow direction, atribuindo a mesma em caso verdadeiro
bool verifyStreamOutlet(int yc, int xc);

//Atualiza o atributo do stream definition
void updateSegments(int row, int column, int rowLast, int columnLast, short attribute);
};
}
}

#endif // CONTINENTAL_HYDRO_TOOLS_STREAMSEGMENTATION_H
