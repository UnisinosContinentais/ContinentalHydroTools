#ifndef CONTINENTAL_HYDRO_TOOLS_STREAMACCUMLENGTH_H
#define CONTINENTAL_HYDRO_TOOLS_STREAMACCUMLENGTH_H

#include <memory>
#include <QString>
#include <continental/datamanagement/Raster.h>

//*******************************************************************
//DETERMINAÇÃO DO COMPRIMENTO ACUMULADO POR SEGMENTO
//Criado por Vinícius Alencar Siqueira - 01/04/2015
//*******************************************************************

using namespace continental::datamanagement;

namespace continental
{
namespace hydrotools
{
/// <summary>
/// Classe que define os trechos de rio
/// </summary>
class StreamAccumLength
{

private:
    std::shared_ptr<datamanagement::Raster<short>> m_strDef;
    std::shared_ptr<datamanagement::Raster<short>> m_flowDirection;
    std::shared_ptr<datamanagement::Raster<float>> m_accumLength;
    std::shared_ptr<datamanagement::Raster<short>> m_strSeg;

public:
    StreamAccumLength();

    //Lê os dados do FlowDirection
    void setFlowDirection(std::shared_ptr<datamanagement::Raster<short>> flowDirection);

    //Lê os dados do FlowDirection
    void setStreamDefinition(std::shared_ptr<datamanagement::Raster<short>> streamDefinition);

    /// <summary>
    /// Separa os trechos de rio em todas as confluências
    /// </summary>
    void computeStreamAccumLength();

    /// <summary>
    /// Segmenta a rede de drenagem de acordo com a máxima
    /// </summary>
    /// <param name="MaxLength">Descrição: o comprimento máximo para cortar trechos (m)</param>
    void segmentStreamsByLength(float maxLength);

private:
    //Atualiza o atributo do stream definition
    void updateReaches(int row, int column, int rowLast, int columnLast, short attribute, int &segmentNumberIncrease, float nReaches, float reachLength);

    bool headwaterFound(short yc, short xc, bool searchAccumLength);


    //Verifica se algum vizinho está apontando FD para
    bool verifyStreamOutlet(short yc, short xc, bool searchAccumLength);
};
}
}

#endif // CONTINENTAL_HYDRO_TOOLS_STREAMACCUMLENGTH_H
