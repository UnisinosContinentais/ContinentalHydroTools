#include <continental/datamanagement/SpatialAnalyst.h>
#include "continental/hydrotools/service/HeuristicSinkRemovalUtil.h"
#include "continental/hydrotools/service/StreamAccumLength.h"
#include "continental/hydrotools/service/FlowDirection.h"

using namespace continental::datamanagement;
using namespace std;

namespace continental
{
namespace hydrotools
{
namespace service
{

StreamAccumLength::StreamAccumLength()
{
    m_flowDirection = make_shared<Raster<short>>();
}

void StreamAccumLength::setFlowDirection(shared_ptr<Raster<short>> flowDirection)
{
    m_flowDirection = flowDirection;
}

void StreamAccumLength::setStreamDefinition(shared_ptr<Raster<short>> streamDefinition)
{
    m_strDef = streamDefinition;
    m_accumLength = make_shared<Raster<float>>(m_strDef->getRows(), m_strDef->getCols(), m_strDef->getXOrigin(), m_strDef->getYOrigin(), m_strDef->getCellSize(), m_strDef->getNoDataValue());
}

void StreamAccumLength::computeStreamAccumLength()
{
    float Length = 0;
    int xAnterior = 0;
    int yAnterior = 0;
    int i = 0;
    int j = 0;
    bool JunctionFound = false;
    float yLat = 0;
    float Xlon = 0;
    int rows = static_cast<int>(m_strDef->getRows()), cols = static_cast<int>(m_strDef->getCols());

    for (int row = 0; row < rows; ++row)
    {
        for (int col = 0; col < cols; ++col)
        {
            i = row;
            j = col;

            if (m_strDef->getData(i, j) == m_strDef->getNoDataValue())
            {
                //Atribui valores de NODATA caso não for drenagem
                m_accumLength->setData(i, j, m_accumLength->getNoDataValue());

            }
            //Caso estiver sobre a drenagem
            else if (m_strDef->getData(i, j) > 0)
            {
                //Caso a célula estiver sobre um trecho que não tenha sido calculado o comprimento
                if (qFuzzyCompare(m_accumLength->getData(i, j), 0))
                {
                    //Se for cabeceira de drenagem
                    if (headwaterFound(i, j, false) == true)
                    {
                        while (true)
                        {
    ReachVerification:
                            //Identifica a linha e a coluna iniciais
                            xAnterior = j;
                            yAnterior = i;
                            //Move em direção à próxima célula
                            HeuristicSinkRemovalUtil::moveToFlowDirection(m_flowDirection->getData(i, j), i, j);

                            //Se alcançar os limites da matriz, Não calcula nada e cai fora do loop
                            if (i < 0 || j < 0 || i > m_strDef->getRows() - 1 || j > m_strDef->getCols() - 1)
                            {
                                //Reinicia o comprimento acumulado
                                Length = 0;
                                break;
                            }

                            if (m_flowDirection->getData(i, j) == m_flowDirection->getNoDataValue())
                            {
                                m_accumLength->setData(i, j, m_accumLength->getNoDataValue());
                                //Reinicia o comprimento acumulado
                                Length = 0;
                                //Passa se mover para um NODATA
                                break;
                            }

                            //Verifica se chegou em uma junção
                            JunctionFound = verifyStreamOutlet((short)i, (short)j, false);
                            if (JunctionFound == true)
                            {

                                //Calcula o comprimento acumulado até a célula anterior à junção
                                Length += SpatialAnalyst::cellLength(yLat, Xlon, (i - yAnterior), (j - xAnterior), m_strDef->getCellSize());
                                //Atribui o comprimento acumulado para a célula anterior (em metros), com base nas direções relativas
                                m_accumLength->setData(yAnterior, xAnterior, Length * 1000);
                                //Reinicia o comprimento acumulado para o novo trecho
                                Length = 0;
                                //Começa a partir do novo trecho
                                goto ReachVerification;
                            }

                            //Calcula a longitude e a latitude da célula em questão
                            yLat = (float)(m_flowDirection->getYOrigin() + (m_flowDirection->getRows() - 1 - i) * m_flowDirection->getCellSize() + (m_flowDirection->getCellSize() / 2));
                            Xlon = (float)(m_flowDirection->getXOrigin() + (j * m_flowDirection->getCellSize()) + (m_flowDirection->getCellSize() / 2));

                            //Calcula o comprimento acumulado até a célula atual, de acordo com
                            Length += SpatialAnalyst::cellLength(yLat, Xlon, (i - yAnterior), (j - xAnterior), m_strDef->getCellSize());
                            //Atribui o comprimento acumulado para a célula anterior (em metros), com base nas direções relativas
                            m_accumLength->setData(yAnterior, xAnterior, Length * 1000);
                        }
                        JunctionFound = false;
                    }
                }
            }
        }
    }
}

void StreamAccumLength::segmentStreamsByLength(float MaxLength)
{
    // A matriz do stream definition é liberada para otimizar a memória
    m_strDef = nullptr;
    m_strSeg = make_shared<Raster<short>>(m_flowDirection->getRows(), m_flowDirection->getCols(), m_flowDirection->getXOrigin(), m_flowDirection->getYOrigin(), m_flowDirection->getCellSize(), m_flowDirection->getNoDataValue());

    float TotalLength = 0;
    int SegmentNumber = 1;
    int IncreasedSegments = 0;
    int xInicial = 0;
    int xFinal = 0;
    int yInicial = 0;
    int yFinal = 0;
    int i = 0;
    int j = 0;
    bool JunctionFound = false;

    for (auto row = 0; row < m_accumLength->getRows(); ++row)
    {
        for (auto col = 0; col < m_accumLength->getCols(); ++col)
        {
            i = row;
            j = col;

            if (m_accumLength->getData(i, j) == m_accumLength->getNoDataValue())
            {
                //Atribui valores de NODATA caso não for drenagem
                m_strSeg->setData(i, j, m_accumLength->getNoDataValue());

            }
            //Caso estiver sobre a drenagem
            else if (m_accumLength->getData(i, j) > 0)
            {
                //Se for cabeceira de drenagem
                if (headwaterFound((short)i, (short)j, true) == true)
                {
                    //Identifica a linha e a coluna iniciais
                    xInicial = j;
                    yInicial = i;

                    while (true)
                    {
    ReachVerification:
                        //Caso a célula estiver sobre um trecho que já tenha sido segmentado, sai do Loop
                        if (m_strSeg->getData(i, j) > 0)
                        {
                            break;
                        }

                        //Identifica a linha e a coluna anteriores
                        xFinal = j;
                        yFinal = i;
                        //Move em direção à próxima célula
                        HeuristicSinkRemovalUtil::moveToFlowDirection(m_flowDirection->getData(i, j), i, j);

                        //Se alcançar os limites da matriz, Não calcula nada e cai fora do loop
                        if (i < 0 || j < 0 || i > m_strSeg->getRows() - 1 || j > m_strSeg->getCols() - 1)
                        {
                            break;
                        }

                        if (m_flowDirection->getData(i, j) == m_flowDirection->getNoDataValue())
                        {
                            //Verifica o comprimento acumulado do trecho
                            TotalLength = m_accumLength->getData(yFinal, xFinal);
                            updateReaches(yInicial, xInicial, i, j, (short)SegmentNumber, IncreasedSegments, (TotalLength / MaxLength), TotalLength);
                            //Aumenta o número dos segmentos identificados
                            SegmentNumber += IncreasedSegments;
                            //Passa se mover para um NODATA
                            break;
                        }

                        //Verifica se chegou em uma junção
                        JunctionFound = verifyStreamOutlet((short)i, (short)j, true);
                        if (JunctionFound == true)
                        {
                            //Verifica o comprimento acumulado do trecho
                            TotalLength = m_accumLength->getData(yFinal, xFinal);
                            updateReaches(yInicial, xInicial, yFinal, xFinal, (short)SegmentNumber, IncreasedSegments, (TotalLength / MaxLength), TotalLength);

                            //Aumenta o número dos segmentos identificados
                            SegmentNumber += IncreasedSegments;

                            //A linha e a coluna iniciais passam a ser do novo trecho
                            xInicial = j;
                            yInicial = i;
                            //Começa a partir do novo trecho
                            goto ReachVerification;

                        }
                    }

                    JunctionFound = false;

                }
            }
        }
    }
}

void StreamAccumLength::updateReaches(int linha, int coluna, int LinhaFinal, int ColunaFinal, short Atributo, int &SegmentNumberIncrease, float nReaches, float ReachLength)
{
    //0 = célula ainda não identificada

    int lin = linha;
    int col = coluna;
    short varAtributo = 1;
    //Retorna o número de trechos (arredonda para cima)
    int nReach_final = std::ceil(nReaches);
    //Identifica o novo limite de comprimento para o trecho
    float maxReachLength = ReachLength / nReach_final;

    //Insere o valor do segmentation final para o "exutório" do trecho
    m_strSeg->setData(LinhaFinal, ColunaFinal, Atributo + (nReach_final - 1));

    //Faça até chegar na célula final do trecho
    while (!(lin == LinhaFinal && col == ColunaFinal))
    {
        m_strSeg->setData(lin, col, Atributo + (varAtributo - 1));
        //Se o comprimento atual for maior do que o permitido para o trecho, aumenta o número do segmento
        if (m_accumLength->getData(lin, col) > (varAtributo * maxReachLength))
        {
            varAtributo += 1;
        }

        HeuristicSinkRemovalUtil::moveToFlowDirection(m_flowDirection->getData(lin, col), lin, col);
        //Cheguei na borda do MDE
        if (col < 0 || lin < 0 || col > (m_flowDirection->getCols() - 1) || lin > (m_flowDirection->getRows() - 1))
        {
            break;
        }
    }

    //Retorna o número de trechos adicionados
    SegmentNumberIncrease = varAtributo;
}

bool StreamAccumLength::headwaterFound(int Yc, int xc, bool SearchAccumLength)
{
    //direções de apontamento para a célula central
    //For y = -1 To 1       2  4  8
    //posY = (Yc + y)       1    x   16
    //For x = -1 To 1       128   64  32
    //posX = (xc + x)

    int xi = 0;
    int yi = 0;
    short nPointingCells = 0;

    for (int y = -1; y <= 1; ++y)
    {
        yi = (Yc + y);
        for (int x = -1; x <= 1; ++x)
        {
            xi = (xc + x);

            //Evita a análise da própria célula (nó central)
            if (x != 0 || y != 0)
            {
                //Evita sair dos limites do raster
                if (xi >= 0 && yi >= 0 && xi < m_flowDirection->getCols() && yi < m_flowDirection->getRows())
                {


                    if (SearchAccumLength == false)
                    {
                        //Somente se a célula faz parte da rede de drenagem principal
                        if (m_strDef->getData(yi, xi) > 0)
                        {
                            //Se a célula analisada apontar para a célula de origem, é contabilizada
                            if (HeuristicSinkRemovalUtil::relativeIncipientFlowDirection(xi, xc, yi, Yc) == m_flowDirection->getData(yi, xi))
                            {
                                nPointingCells += 1;
                            }
                        }
                    }
                    else
                    {
                        //Somente se a célula faz parte da rede de drenagem principal
                        if (m_accumLength->getData(yi, xi) > 0)
                        {
                            //Se a célula analisada apontar para a célula de origem, é contabilizada
                            if (HeuristicSinkRemovalUtil::relativeIncipientFlowDirection(xi, xc, yi, Yc) == m_flowDirection->getData(yi, xi))
                            {
                                nPointingCells += 1;
                            }
                        }
                    }

                }
            }
        }
    }

    //Se nenhuma célula apontar para a célula central, significa que encontrou uma cabeceira
    return nPointingCells == 0;
}

bool StreamAccumLength::verifyStreamOutlet(int Yc, int xc, bool SearchAccumLength)
{
    //direções de apontamento para a célula central
    //For y = -1 To 1       2  4  8
    //posY = (Yc + y)       1    x   16
    //For x = -1 To 1       128   64  32
    //posX = (xc + x)

    int xi = 0;
    int yi = 0;
    short nPointingCells = 0;
    int rows = static_cast<int>(m_flowDirection->getRows()), cols = static_cast<int>(m_flowDirection->getCols());

    for (int y = -1; y <= 1; ++y)
    {
        yi = (Yc + y);
        for (int x = -1; x <= 1; ++x)
        {
            xi = (xc + x);

            //Evita a análise da própria célula (nó central)
            if (x != 0 || y != 0)
            {
                //Evita sair dos limites do raster
                if (xi >= 0 && yi >= 0 && xi < cols && yi < rows)
                {
                    if (SearchAccumLength == false)
                    {
                        //Somente se a célula faz parte da rede de drenagem principal
                        if (m_strDef->getData(yi, xi) > 0)
                        {
                            //Se a célula analisada apontar para a célula de origem, é contabilizada
                            if (HeuristicSinkRemovalUtil::relativeIncipientFlowDirection(xi, xc, yi, Yc) == m_flowDirection->getData(yi, xi))
                            {
                                nPointingCells += 1;
                            }
                        }
                    }
                    else
                    {
                        //Somente se a célula faz parte da rede de drenagem principal
                        if (m_accumLength->getData(yi, xi) > 0)
                        {
                            //Se a célula analisada apontar para a célula de origem, é contabilizada
                            if (HeuristicSinkRemovalUtil::relativeIncipientFlowDirection(xi, xc, yi, Yc) == m_flowDirection->getData(yi, xi))
                            {
                                nPointingCells += 1;
                            }
                        }
                    }

                }
            }
        }
    }

    //Se a mais de uma célula apontar para a célula central, significa que encontrou uma confluência
    return nPointingCells > 1;
}

}
}
}
