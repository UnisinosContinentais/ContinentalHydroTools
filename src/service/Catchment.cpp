#include <continental/datamanagement/Raster.h>

#include "continental/hydrotools/service/Catchment.h"
#include "continental/hydrotools/service/Cell.h"
#include "continental/hydrotools/service/CellWatershed.h"
#include "continental/hydrotools/service/FlowDirection.h"
#include "continental/hydrotools/service/HeuristicSinkRemovalUtil.h"
#include "continental/hydrotools/service/shape/ShapeFile.h"
#include "continental/hydrotools/service/shape/ShapeObject.h"

using namespace std;
using namespace continental::datamanagement;
using namespace continental::hydrotools::service::shape;

namespace continental {
namespace hydrotools {
namespace service {

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

size_t Catchment::getNumberCellsBasin() const
{
    return m_numberCellsBasin;
}

Catchment::Catchment()
{
}

void Catchment::setStreamSegmentation(shared_ptr<Raster<short>> streamSegmentation)
{
    //Pega os dados de stream segment e joga na matriz de watershed para marcar pontos na bacia
    m_waterShed = streamSegmentation;
}

void Catchment::setPointOutlets(const QString &shapeFileOutlets)
{
    shape::ShapeFile shapeFile;
    shapeFile.Open(shapeFileOutlets);

    // atribui o número de exutórios
    size_t countCellExhilarating = static_cast<size_t>(shapeFile.GetEntityCount());
    m_CellExhilarating = make_shared<vector<shared_ptr<CellWatershed>>>();
    m_CellExhilarating->resize(countCellExhilarating);

    for (size_t i = 0; i < countCellExhilarating; ++i)
    {
        ShapeObject object;
        shapeFile.GetShape(i, object);
        auto point = object.getVertices()->at(0);
        (*m_CellExhilarating)[i] = make_shared<CellWatershed>(point->y(), point->x(), m_flowDirection->getRows(), m_flowDirection->getCols(), m_flowDirection->getCellSize(), m_flowDirection->getXOrigin(), m_flowDirection->getYOrigin(), i + 1);
    }

    // Insere os exutórios na matriz de dados
    insertOutlets();
}

void Catchment::setPointOutlets(const QString &ShapeFileOutlets, size_t index)
{
    //Especial para Ferramenta "Depth-Area-Volume", analisando os vários pontos de um shapefile um de cada vez
    shape::ShapeFile shapeFile;
    shapeFile.Open(ShapeFileOutlets);

    size_t countCellExhilarating = static_cast<size_t>(shapeFile.GetEntityCount());
    m_CellExhilarating = make_shared<vector<shared_ptr<CellWatershed>>>();
    m_CellExhilarating->resize(countCellExhilarating);

    ShapeObject object;
    shapeFile.GetShape(index, object);
    auto point = object.getVertices()->at(0);
    (*m_CellExhilarating)[index] = make_shared<CellWatershed>(point->y(), point->x(), m_flowDirection->getRows(), m_flowDirection->getCols(), m_flowDirection->getCellSize(), m_flowDirection->getXOrigin(), m_flowDirection->getYOrigin(), index);

    //Insere os exutórios na matriz de dados
    insertOutlets();
}

void Catchment::setPointOutlets(std::vector<std::pair<double, double>> &vectorPairLatitudeLongitude)
{
    auto limit = vectorPairLatitudeLongitude.size();
    m_CellExhilarating = make_shared<vector<shared_ptr<CellWatershed>>>();
    m_CellExhilarating->resize(limit);

    for (size_t i = 0; i < limit; ++i)
    {
        (*m_CellExhilarating)[i] = make_shared<CellWatershed>(vectorPairLatitudeLongitude[i].first, vectorPairLatitudeLongitude[i].second, m_flowDirection->getRows(), m_flowDirection->getCols(), m_flowDirection->getCellSize(), m_flowDirection->getXOrigin(), m_flowDirection->getYOrigin(), i + 1);
    }

    // Insere os exutórios na matriz de dados
    insertOutlets();
}

void Catchment::findWatersheds()
{
    identifiesWatershed();
}

void Catchment::identifiesWatershed()
{
    m_numberCellsBasin = 0;

    int limitRows = static_cast<int>(m_flowDirection->getRows() - 1);
    int limitCols = static_cast<int>(m_flowDirection->getCols() - 1);

    for (int i = 0; i <= limitRows; ++i)
    {
        int y = i;
        for (int j = 0; j <= limitCols; ++j)
        {
            int x = j;

            //Identificar a célula de partida para atualizar a matriz Watershed
            int yBegin = y;
            int xBegin = x;

            while (true)
            {
                //Identifica a posição da célula antes de mover-se na matriz
                int xPrevious = x;
                int yPrevious = y;

                if (m_flowDirection->getData(y, x) == m_flowDirection->getNoDataValue())
                {
                    updateWaterShed(yBegin, xBegin, static_cast<short>(m_flowDirection->getNoDataValue()));
                    break;
                }

                //Identifica a célula seguinte, para onde está direcionado o fluxo
                HeuristicSinkRemovalUtil::moveToFlowDirection(m_flowDirection->getData(y, x), y, x);

                //Cheguei na borda do MDE
                if (x < 0 || y < 0 || x > (limitCols) || y > limitRows)
                {
                    updateWaterShed(yBegin, xBegin, m_flowDirection->getNoDataValue());
                    break;
                }
                //Cheguei em um NODATA; o caminho ao longo da matriz deverá ser = NODATA
                else if (x == xPrevious && y == yPrevious)
                {
                    updateWaterShed(yBegin, xBegin, m_flowDirection->getNoDataValue());
                    break;
                }
                //Cheguei em um dos exutórios
                else if (m_waterShed->getData(y, x) > 0)
                {
                    updateWaterShed(yBegin, xBegin, m_waterShed->getData(y, x));
                    break;
                }

                //Se encontrar um noData pelo caminho é sinal de que este caminho não dará em lugar algum
                if (m_waterShed->getData(y, x) == m_flowDirection->getNoDataValue())
                {
                    updateWaterShed(yBegin, xBegin, m_flowDirection->getNoDataValue());
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
    for (size_t i = 0; i < m_CellExhilarating->size(); ++i)
    {
        const auto row = (*m_CellExhilarating)[i]->y;
        const auto column = (*m_CellExhilarating)[i]->x;
        const auto attribute = (*m_CellExhilarating)[i]->getAttribute();
        m_waterShed->setData(row, column, attribute);
    }
}

void Catchment::insertOutletByRowCol(const size_t row, const size_t column)
{
    if (m_CellExhilarating == nullptr)
    {
        m_CellExhilarating = make_shared<vector<shared_ptr<CellWatershed>>>();
    }

    const auto index = (*m_CellExhilarating).size();
    (*m_CellExhilarating).resize(index + 1);
    const auto cell = make_shared<CellWatershed>(0, 0, m_flowDirection->getRows(), m_flowDirection->getCols(), m_flowDirection->getCellSize(), m_flowDirection->getXOrigin(), m_flowDirection->getYOrigin(), index + 1);
    cell->x = column;
    cell->y = row;
    (*m_CellExhilarating)[index] = cell;
    const auto attribute = cell->getAttribute();
    m_waterShed->setData(row, column, attribute);
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

            m_waterShed->setData(static_cast<size_t>(row), static_cast<size_t>(column), attribute);
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
}
