#include <continental/datamanagement/Raster.h>
#include <continental/datamanagement/RasterFile.h>

#include "continental/hydrotools/Catchment.h"
#include "continental/hydrotools/Cell.h"
#include "continental/hydrotools/CellWatershed.h"
#include "continental/hydrotools/FlowDirection.h"
#include "continental/hydrotools/HeuristicSinkRemovalUtil.h"
#include "continental/hydrotools/shape/ShapeFile.h"
#include "continental/hydrotools/shape/ShapeObject.h"

using namespace std;
using namespace continental::datamanagement;
using namespace continental::hydrotools::shape;

namespace continental
{
namespace hydrotools
{

shared_ptr<Raster<short>> Catchment::getWaterShed() const
{
    return m_waterShed;
}

shared_ptr<Raster<short>> Catchment::getFlowDirection() const
{
    return m_flowDirection;
}

void Catchment::setFlowDirection(shared_ptr<Raster<short>> flowDirection)
{
    m_flowDirection = flowDirection;
    m_waterShed = make_shared<Raster<short>>(m_flowDirection.get()->getRows(), m_flowDirection->getCols(), m_flowDirection->getXOrigin(), m_flowDirection->getYOrigin(), m_flowDirection->getCellSize(), m_flowDirection->getNoDataValue());
}

shared_ptr<vector<shared_ptr<CellWatershed>>> Catchment::getCellsExhilarating() const
{
    return m_CellExhilarating;
}

size_t Catchment::getNumberCellsBasin() const
{
    return m_numberCellsBasin;
}

Catchment::Catchment()
{
}

void Catchment::readFlowDirectionData(const QString &fileName)
{
    m_flowDirection = make_shared<Raster<short>>(RasterFile<short>::loadRasterByFile(fileName));
    m_waterShed = make_shared<Raster<short>>(m_flowDirection->getRows(), m_flowDirection->getCols(), m_flowDirection->getXOrigin(), m_flowDirection->getYOrigin(), m_flowDirection->getCellSize(), m_flowDirection->getNoDataValue());
}

void Catchment::readStreamSegmentData(const QString &fileName)
{
    //Pega os dados de stream segment e joga na matriz de watershed para marcar pontos na bacia
    m_waterShed = make_shared<Raster<short>>(RasterFile<short>::loadRasterByFile(fileName));
}

void Catchment::setPointOutlets(const QString &shapeFileOutlets)
{
    m_shapefile.Open(shapeFileOutlets);

    // atribui o número de exutórios
    size_t countCellExhilarating = static_cast<size_t>(m_shapefile.GetEntityCount());
    m_CellExhilarating->resize(countCellExhilarating);

    for (size_t i = 0; i < countCellExhilarating; i++)
    {
        ShapeObject object;
        m_shapefile.GetShape(i, object);
        auto point = object.getVertices()->at(0);
        (*m_CellExhilarating)[i] = make_shared<CellWatershed>(point->y(), point->x(), m_flowDirection->getRows(), m_flowDirection->getCols(), m_flowDirection->getCellSize(), m_flowDirection->getXOrigin(), m_flowDirection->getYOrigin(), i);
    }

    // Insere os exutórios na matriz de dados
    insertOutlets();

}

void Catchment::setPointOutlets(const QString &ShapeFileOutlets, size_t index)
{
    //Especial para Ferramenta "Depth-Area-Volume", analisando os vários pontos de um shapefile um de cada vez
    m_shapefile.Open(ShapeFileOutlets);

    size_t countCellExhilarating = static_cast<size_t>(m_shapefile.GetEntityCount());
    m_CellExhilarating->resize(countCellExhilarating);

    ShapeObject object;
    m_shapefile.GetShape(index, object);
    auto point = object.getVertices()->at(0);
    (*m_CellExhilarating)[index] = make_shared<CellWatershed>(point->y(), point->x(), m_flowDirection->getRows(), m_flowDirection->getCols(), m_flowDirection->getCellSize(), m_flowDirection->getXOrigin(), m_flowDirection->getYOrigin(), index);

    //Insere os exutórios na matriz de dados
    insertOutlets();
}

void Catchment::findWatersheds()
{
    //If _CellExutorios Is Nothing Then Throw New Exception("Não existem pontos de exutório!")
    identifiesWatershed();
}

void Catchment::identifiesWatershed()
{
    m_numberCellsBasin = 0;

    int limitRows = static_cast<int>(m_flowDirection->getRows() - 1);
    int limitCols = static_cast<int>(m_flowDirection->getCols() - 1);

    for (int i = 0; i <= limitCols; i++)
    {
        int y = i;
        for (int j = 0; j <= limitRows; j++)
        {
            int x = j;

            //Identificar a célula de partida para atualizar a matriz Watershed
            int yInicial = y;
            int xInicial = x;

            while (true)
            {
                //Identifica a posição da célula antes de mover-se na matriz
                int xAnterior = x;
                int yAnterior = y;
                if (m_flowDirection->getData(x, y) == m_flowDirection->getNoDataValue())
                {
                    updateWaterShed(yInicial, xInicial, static_cast<short>(m_flowDirection->getNoDataValue()));
                    break;
                }

                //Identifica a célula seguinte, para onde está direcionado o fluxo
                HeuristicSinkRemovalUtil::moveToFlowDirection(m_flowDirection->getData(y, x), y, x);

                //Cheguei na borda do MDE
                if (x < 0 || y < 0 || x > (limitCols) || y > limitRows)
                {
                    updateWaterShed(yInicial, xInicial, m_flowDirection->getNoDataValue());
                    break;

                }
                //Cheguei em um NODATA; o caminho ao longo da matriz deverá ser = NODATA
                else if (x == xAnterior && y == yAnterior)
                {
                    updateWaterShed(yInicial, xInicial, m_flowDirection->getNoDataValue());
                    break;

                }
                //Cheguei em um dos exutórios
                else if (m_waterShed->getData(y, x) > 0)
                {
                    updateWaterShed(yInicial, xInicial, m_waterShed->getData(y, x));
                    break;
                }

                //Se encontrar um noData pelo caminho é sinal de que este caminho não dará em lugar algum
                if (m_waterShed->getData(y, x) == m_flowDirection->getNoDataValue())
                {
                    updateWaterShed(yInicial, xInicial, m_flowDirection->getNoDataValue());
                    break;
                }

                //Se o atributo for igual a 0 na matriz watershed, continua seguindo através das direções de fluxo
            }
            //retorna o y para a linha atual
            y = i;
        }
    }

}

void Catchment::insertOutlets()
{

    //Insere em cada exutório o valor do atributo, que corresponderá à Sub-bacia
    for (size_t i = 0; i < m_CellExhilarating->size(); i++)
    {
        m_waterShed->setData((*m_CellExhilarating)[i]->y, (*m_CellExhilarating)[i]->x, static_cast<short>((*m_CellExhilarating)[i]->getAttribute()));
    }

}

void Catchment::updateWaterShed(int row, int column, short attribute)
{
    //0 = célula ainda não identificada

    int limitCols = static_cast<int>(m_flowDirection->getCols() - 1);
    int limitRows = static_cast<int>(m_flowDirection->getRows() - 1);

    //Enquanto nada tiver sido verificado
    while (true)
    {
        //Retirado por Vinícius Siqueira 09/07/2015
        if (m_waterShed->getData(row, column) == 0)
        {
            //If _WaterShed.Dados(lin, col) = _WaterShed.NoDataValue Then 'Modificado por Vinícius Siqueira 09/07/2015 (Compatibilização com o NoData da bacia)

            m_waterShed->setData(row, column, attribute);
            m_numberCellsBasin += 1;
            HeuristicSinkRemovalUtil::moveToFlowDirection(m_flowDirection->getData(row, column), row, column);
            //Cheguei na borda do MDE
            if (column < 0 || row < 0 || column > limitCols || row > limitRows)
            {
                break;
            }

        }
        else
        {
            //Já identifiquei esta célula com algum identificador, não preciso verificar denovo o caminho
            break;
        }
    }

}

}
}
