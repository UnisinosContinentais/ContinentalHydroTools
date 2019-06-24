#ifndef CONTINENTAL_HYDRO_TOOLS_CATCHMENTS_H
#define CONTINENTAL_HYDRO_TOOLS_CATCHMENTS_H

#include <QString>
#include <vector>
#include <memory>

#include <continental/datamanagement/Raster.h>
#include "continental/hydrotools/CellWatershed.h"
// #include "continental/hydrotools/shape/ShapeFile.h"

//*******************************************************************
//DETERMINAÇÃO DE BACIAS
//*******************************************************************

namespace continental
{
namespace hydrotools
{

/// <summary>
/// Classe para identificação de bacias hidrográficas a partir de exutórios sobre a a rede de drenagem.
/// </summary>
/// <remarks></remarks>
class Catchment
{
private:
//Raster com FlowDirection
std::shared_ptr<datamanagement::Raster<short>> m_flowDirection;
//Células com a posição dos exutórios
std::shared_ptr<std::vector<std::shared_ptr<CellWatershed>>> m_CellExhilarating;
size_t m_numberCellsBasin = 0;
// shape::ShapeFile m_shapefile;

protected:
std::shared_ptr<datamanagement::Raster<short>> m_waterShed;

public:
    std::shared_ptr<datamanagement::Raster<short>> getWaterShed() const;

    /// <summary>
    /// Retorna o Flow Direction
    /// </summary>
    std::shared_ptr<datamanagement::Raster<short>> getFlowDirection() const;

    void setFlowDirection(std::shared_ptr<datamanagement::Raster<short>> flowDirection);

    /// <summary>
    /// Retorna as células com a posição dos exutórios
    /// </summary>
    std::shared_ptr<std::vector<std::shared_ptr<CellWatershed>>> getCellsExhilarating() const;

    size_t getNumberCellsBasin() const;

    /// <summary>
    /// Cria uma nova instância da class
    /// </summary>
    Catchment();

    virtual ~Catchment() = default;

    //Lê os dados do FlowDirection
    void readFlowDirectionData(const QString &local);

    //Lê os dados do StreamSegmentation
    void readStreamSegmentData(const QString &local);

    /// <summary>
    /// Atribui os exutóros na matriz
    /// </summary>
    /// <param name="ShapeFileOutlets">Descrição: O arquivo em formato shapefile com os exutorios</param>
    void setPointOutlets(const QString &shapeFileOutlets);

    /// <summary>
    /// Atribui os exutóros na matriz para Ferramenta "Depth-Area-Volume"
    /// </summary>
    /// <param name="ShapeFileOutlets">Descrição: O arquivo em formato shapefile com os exutorios</param>
    void setPointOutlets(const QString &shapeFileOutlets, size_t index);

    void setPointOutlets(std::vector<std::pair<double, double>> &vectorPairLatitudeLongitude);

    /// <summary>
    /// Identifica as catchments
    /// </summary>
    virtual void findWatersheds();

    //Identifica a bacia hidrográfica
private:
    void identifiesWatershed();

    //Insere os pontos dos exutórios especificados pelo shapefile de entrada
    void insertOutlets();

    //Insere o valor do atributo na linha e coluna especificada, na matriz watershed
    //Modificado para Long para rodar o South America
    void updateWaterShed(int row, int column, short attribute);
};

}
}

#endif // CONTINENTAL_HYDRO_TOOLS_CATCHMENTS_H
