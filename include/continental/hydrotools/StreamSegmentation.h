#ifndef IPHYDRORASTERTOOLS_STREAMSEGMENTATION_H
#define IPHYDRORASTERTOOLS_STREAMSEGMENTATION_H

#include <QString>
#include <memory>
#include <continental/datamanagement/Raster.h>

//*******************************************************************
//DETERMINAÇÃO DO STREAM SEGMENTATION
//Criado por Vinícius Alencar Siqueira - 20/01/2014
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

//Lê os dados do FlowDirection
void readFlowDirectionData(const QString &fileName);

//Lê os dados do FlowDirection
void readStreamDefinitionData(const QString &fileName);

void setFlowDirectionData(std::shared_ptr<datamanagement::Raster<short>> flowDirection);

void setStreamDefinition(std::shared_ptr<datamanagement::Raster<short>> streamDefinition);

/// <summary>
/// Separa os trechos de rio em todas as confluências
/// </summary>
void segmentStreams();

private:
//Verifica se algum vizinho de mesma cota, ou inferior já possui flow direction, atribuindo a mesma em caso verdadeiro
bool verifyStreamOutlet(int yc, int xc);

//Atualiza o atributo do stream definition
void updateSegments(int row, int column, int rowLast, int columnLast, short attribute);
};
}
}

#endif // IPHYDRORASTERTOOLS_STREAMSEGMENTATION_H
