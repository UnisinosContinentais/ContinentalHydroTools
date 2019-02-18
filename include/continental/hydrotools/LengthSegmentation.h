#ifndef CONTINENTAL_HYDRO_TOOLS_LENGTHSEGMENTATION_H
#define CONTINENTAL_HYDRO_TOOLS_LENGTHSEGMENTATION_H

#include <QString>
#include <vector>
#include <memory>
#include <continental/datamanagement/Raster.h>
#include "continental/hydrotools/JunctionCell.h"

//*******************************************************************
//SEGMENTAÇÃO DE TRECHOS POR COMPRIMENTO MÍNIMO
//Criado por Vinícius Alencar Siqueira - 10/05/2015
//*******************************************************************

namespace continental
{
namespace hydrotools
{
/// <summary>
/// Classe que define os trechos de rio
/// </summary>
class LengthSegmentation
{
private:
    std::shared_ptr<continental::datamanagement::Raster<short>> m_flowDirection;
    std::shared_ptr<continental::datamanagement::Raster<float>> m_flowAcc;
    //Private _StrSeg As RasterReal 'FMF em 12/01/2016
    std::shared_ptr<continental::datamanagement::Raster<short>> m_strSeg;
    std::vector<std::unique_ptr<JunctionCell>> m_junctionCells;
    std::vector<float> m_junctionCellsAreas;
    std::vector<bool> m_cellAnalyzed;
    int m_numberOfJunctionCells = 0;
    float m_minLength = 0;
    int m_xOutlet = 0;
    int m_yOutlet = 0;
    //Número de junções como chute inicial
    size_t m_guessJunctionNumber = 10000;

public:
    LengthSegmentation(float minLenght);

    //Lê os dados do FlowDirection
    void setFlowDirection(std::shared_ptr<continental::datamanagement::Raster<short>> flowDirection);

    //Lê os dados do FlowAccumulation
    void setFlowAccumulation(std::shared_ptr<continental::datamanagement::Raster<float>> flowAccumulation);

    //Lê os dados do StreamDefinition e guarda no segmentation
    void setStreamDefinition(std::shared_ptr<continental::datamanagement::Raster<short>> streamDefinition);

    /// <summary>
    /// Identifica as Junções e marcas os possíveis exutórios
    /// </summary>
    void identifyJunctionsAndOutlet();
private:
    void identifyOutlets(); //Pedro 13/01/16 - Identifica exutórios pelas direções de fluxo

    //Verifica se algum vizinho está apontando FD para
    void identifyAndMarkJunctions(short yc, short xc);
public:
    /// <summary>
    /// Segmenta a rede por um comprimento máximo, não levando em consideração as junções da rede.
    /// </summary>
    void segmentBySpecificLength();

private:
    //Move célula rio acima
    void moveToUpstream(short yc, short xc, short &rowUpstr, short &colUpstr, bool forceHeadwater);

public:
    /// <summary>
    /// Segmenta a rede por um comprimento máximo, não levando em consideração as junções da rede.
    /// </summary>
    void doubleSegmentByLength();

private:
    //Move célula rio acima
    void moveToUpstreamDoubleSegmented(short yc, short xc, short &rowUpstr, short &colUpstr, bool forceHeadwater, float actualLength, int segmentValue);

public:
    //Realiza o pós processamento, para retirada de múltiplos trechos dentro de uma mesma minibacia
    void doubleSegmentedPosProcessing();

private:
    //Move célula rio acima
    void moveToUpstreamMinArea(short yc, short xc, short &rowUpstr, short &colUpstr, bool &afterJunction, bool markOutlets);

    /// <summary>
    /// Identifica a posição da célula que corresponde à cordenada indicada
    /// </summary>
    /// <param name="y"></param>
    /// <param name="x"></param>
    long long findCellPosition(short y, short x);

public:
    //Escreve os dados da matrix dos trechos segmentados
    void writeSegmentedStreams(const QString &file);

};
}
}

#endif // CONTINENTAL_HYDRO_TOOLS_LENGTHSEGMENTATION_H
