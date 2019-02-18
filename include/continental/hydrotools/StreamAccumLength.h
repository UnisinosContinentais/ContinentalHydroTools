#ifndef IPHYDRORASTERTOOLS_STREAMACCUMLENGTH_H
#define IPHYDRORASTERTOOLS_STREAMACCUMLENGTH_H

#include <QString>
#include "continental/dataManagement/Raster.h"

//*******************************************************************
//DETERMINAÇÃO DO COMPRIMENTO ACUMULADO POR SEGMENTO
//Criado por Vinícius Alencar Siqueira - 01/04/2015
//*******************************************************************

using namespace continental::dataManagement;

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
    Raster<short> *m_strDef;
    Raster<short> *m_flowDirection;
    Raster<float> *m_accumLength;
    Raster<short> *m_strSeg;

public:	
    virtual ~StreamAccumLength()
    {
        delete m_strDef;
        delete m_flowDirection;
        delete m_accumLength;
        delete m_strSeg;
    }

    StreamAccumLength();

    //Lê os dados do FlowDirection
    void readFlowDirectionData(const QString &file);

    //Lê os dados do FlowDirection
    void readStreamDefinitionData(const QString &file);

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

public:
    //Escreve os dados da matrix dos comprimentos acumulados
    void writeAccumLengthData(const QString &file);

    //Escreve os dados da matrix dos trechos segmentados
    void WriteSegmentedStreams(const QString &file);
};
}
}

#endif // IPHYDRORASTERTOOLS_STREAMACCUMLENGTH_H
