#include <continental/datamanagement/SpatialAnalyst.h>
#include "continental/hydrotools/LengthSegmentation.h"
#include "continental/hydrotools/FlowDirection.h"
#include "continental/hydrotools/HeuristicSinkRemovalUtil.h"

using namespace continental::datamanagement;
using namespace std;

namespace continental
{
namespace hydrotools
{
LengthSegmentation::LengthSegmentation(float minLenght)
{
    m_flowDirection = make_shared<Raster<short>>();
    m_minLength = minLenght;
}

void LengthSegmentation::setFlowDirection(shared_ptr<Raster<short>> flowDirection)
{
    m_flowDirection = flowDirection;
}

void LengthSegmentation::setFlowAccumulation(shared_ptr<Raster<float>> flowAccumulation)
{
    m_flowAcc = flowAccumulation;
}

void LengthSegmentation::setStreamDefinition(shared_ptr<Raster<short>> streamDefinition)
{
    //_StrSeg = New RasterReal  'FMF em 12/01/2016
    m_strSeg = streamDefinition;
}

void LengthSegmentation::identifyJunctionsAndOutlet()
{

    float maxAccum = 0;
    int i = 0;
    int j = 0;
    int xAnterior = 0;
    int yAnterior = 0;
    //Redimensiona a matriz de células analisadas
    m_cellAnalyzed.clear();
    m_cellAnalyzed.resize(m_strSeg->getRows() * m_strSeg->getCols());

    m_junctionCells.clear();
    m_junctionCells.resize(m_guessJunctionNumber);

    m_junctionCellsAreas.clear();
    m_junctionCellsAreas.resize(m_guessJunctionNumber);

    m_numberOfJunctionCells = -1;

    auto rows = static_cast<int>(m_strSeg->getRows()), cols = static_cast<int>(m_strSeg->getCols());
    for (int row = 0; row < rows; row++)
    {
        for (int col = 0; col < cols; col++)
        {
            i = row;
            yAnterior = row;
            j = col;
            xAnterior = col;

            //Caso estiver sobre a drenagem
            if (m_strSeg->getData(i, j) > 0)
            {
                //Identifica possíveis junções e marca os exutórios
                identifyAndMarkJunctions(i, j);

                //Identificação da célula com exutório
                if (m_flowAcc->getData(i, j) > maxAccum)
                {
                    m_yOutlet = i;
                    m_xOutlet = j;
                    maxAccum = m_flowAcc->getData(i, j);
                }

                while (true)
                {
                    //Move em direção à próxima célula
                    HeuristicSinkRemovalUtil::moveToFlowDirection(m_flowDirection->getData(i, j), i, j);

                    //Se alcançar os limites da matriz, Não calcula nada e cai fora do loop
                    if (i < 0 || j < 0 || i > rows - 1 || j > cols - 1)
                    {
                        break;
                    }

                    //Identificação da célula com exutório
                    if (m_flowAcc->getData(i, j) > maxAccum)
                    {
                        m_yOutlet = i;
                        m_xOutlet = j;
                        maxAccum = m_flowAcc->getData(i, j);
                    }

                    //Identifica possíveis junções e marca os exutórios
                    identifyAndMarkJunctions(i, j);
                    m_cellAnalyzed[static_cast<size_t>(yAnterior * cols + xAnterior)] = true;

                    //Se a célula já foi analisada, sai do loop
                    if (m_cellAnalyzed[static_cast<size_t>(i * cols + j)] == true)
                    {
                        break;
                    }

                    yAnterior = i;
                    xAnterior = j;

                    if (m_flowDirection->getData(i, j) == m_flowDirection->getNoDataValue())
                    {
                        //Achou o exutório da bacia
                        //Passa se mover para um NODATA
                        break;
                    }
                }
            }

        }
    }

    //   _NumberOfJunctionCells += 1 'Para acrescentar o exutório da bacia

    // ReDim Preserve _JunctionCells(_NumberOfJunctionCells) 'Deixa o vetor de células de junção com a dimensão correta
    // ReDim Preserve _JunctionCellsAreas(_NumberOfJunctionCells) 'Deixa o vetor de áreas com a dimensão correta

    //Acrescenta o exutório da bacia
    //_JunctionCells(_NumberOfJunctionCells) = New JunctionCell(_yOutlet, _xOutlet)
    //_JunctionCellsAreas(_NumberOfJunctionCells) = maxAccum

    identifyOutlets(); //Pedro 13/01/16

    //Reordena as matriz de acordo com a área dos exutórios
    // Array::Sort(_JunctionCellsAreas, _JunctionCells);

    //Adicionar neste ponto uma rotina que acha todos os outlets da bacia e marca eles como junctions
    //Esta rotina deve percorrer todo o fdr e achar todas as células que drenam para um ponto NoData e possuam valor de Stream. Imediatamente estas células são marcadas como junctions
    //Siqueira e Fernando em 12/01/2016 às 16:41h
}

void LengthSegmentation::identifyOutlets()
{
    int yAnterior = 0, xAnterior = 0, i = 0, j = 0;
    int rows = static_cast<int>(m_strSeg->getRows()), cols = static_cast<int>(m_strSeg->getCols());

    for (int row = 0; row < rows; row++)
    {
        for (int col = 0; col < cols; col++)
        {
            i = row;
            yAnterior = row;
            j = col;
            xAnterior = col;

            if (m_strSeg->getData(i, j) > 0)
            {
                HeuristicSinkRemovalUtil::moveToFlowDirection(m_flowDirection->getData(i, j), i, j);

                if (i < 0 || j < 0 || i > rows - 1 || j > cols - 1 || m_flowDirection->getData(i, j) == m_flowDirection->getNoDataValue())
                {
                    m_numberOfJunctionCells += 1;

                    //Caso a matriz precise ser maior do que o chute inicial
                    if (m_numberOfJunctionCells > static_cast<int>(m_guessJunctionNumber))
                    {
                        //Dobra o tamanho da matriz e preserva os itens
                        m_guessJunctionNumber = m_guessJunctionNumber * 2;
                        m_junctionCells.resize(m_guessJunctionNumber);
                        m_junctionCellsAreas.resize(m_guessJunctionNumber);
                    }

                    //Adiciona o exutório do contribuinte
                    m_junctionCells[static_cast<size_t>(m_numberOfJunctionCells)] = make_unique<JunctionCell>(yAnterior, xAnterior);
                    //Adiciona a área acumulada para as células
                    m_junctionCellsAreas[static_cast<size_t>(m_numberOfJunctionCells)] = m_flowAcc->getData(yAnterior, xAnterior);
                }
            }
        }
    }

    //Deixa o vetor de células de junção com a dimensão correta
    m_junctionCells.resize(static_cast<size_t>(m_numberOfJunctionCells));
    //Deixa o vetor de áreas com a dimensão correta
    m_junctionCellsAreas.resize(static_cast<size_t>(m_numberOfJunctionCells));
}

void LengthSegmentation::identifyAndMarkJunctions(short Yc, short xc)
{
    //direções de apontamento para a célula central
    //For y = -1 To 1       2  4  8
    //posY = (Yc + y)       1    x   16
    //For x = -1 To 1       128   64  32
    //posX = (xc + x)

    int xi = 0;
    int yi = 0;
    short nPointingCells = 0;
    auto rows = static_cast<int>(m_flowDirection->getRows()), cols = static_cast<int>(m_flowDirection->getCols());

    for (int y = -1; y <= 1; y++)
    {
        yi = (Yc + y);
        for (int x = -1; x <= 1; x++)
        {
            xi = (xc + x);

            //Evita a análise da própria célula (nó central)
            if (x != 0 || y != 0)
            {
                //Evita sair dos limites do raster
                if (xi >= 0 && yi >= 0 && xi < cols && yi < rows)
                {
                    //Somente se a célula faz parte da rede de drenagem principal
                    if (m_strSeg->getData(yi, xi) > 0)
                    {
                        if (HeuristicSinkRemovalUtil::relativeIncipientFlowDirection(xi, xc, yi, Yc) == m_flowDirection->getData(yi, xi))
                        {
                            nPointingCells += 1;
                        }
                    }
                }
            }
        }
    }

    if (nPointingCells > 1) //Caso tenha mais que uma célula apontando para a célula central
    {
        for (int y = -1; y <= 1; y++)
        {
            yi = (Yc + y);
            for (int x = -1; x <= 1; x++)
            {
                xi = (xc + x);

                //Evita a análise da própria célula (nó central)
                if (x != 0 || y != 0)
                {
                    //Evita sair dos limites do raster
                    if (xi >= 0 && yi >= 0 && xi < cols && yi < rows)
                    {
                        //Somente se a célula faz parte da rede de drenagem principal
                        if (m_strSeg->getData(yi, xi) > 0)
                        {
                            if (HeuristicSinkRemovalUtil::relativeIncipientFlowDirection(xi, xc, yi, Yc) == m_flowDirection->getData(yi, xi))
                            {

                                //Adiciona a célula somente se ela não tenha sido adicionada ainda
                                if (m_cellAnalyzed[yi * m_strSeg->getCols() + xi] == false)
                                {
                                    m_numberOfJunctionCells += 1;
                                    //Caso a matriz precise ser maior do que o chute inicial
                                    if (m_numberOfJunctionCells > m_guessJunctionNumber)
                                    {
                                        //Dobra o tamanho da matriz e preserva os itens
                                        m_guessJunctionNumber = m_guessJunctionNumber * 2;
                                        m_junctionCells.resize(m_guessJunctionNumber);
                                        m_junctionCellsAreas.resize(m_guessJunctionNumber);
                                    }

                                    //Adiciona o exutório do contribuinte
                                    m_junctionCells[m_numberOfJunctionCells] = make_unique<JunctionCell>(yi, xi);
                                    //Adiciona a área acumulada para as células
                                    m_junctionCellsAreas[m_numberOfJunctionCells] = m_flowAcc->getData(yi, xi);
                                    //Marca a célula adicionada como já sendo analisada
                                    m_cellAnalyzed[yi * cols + xi] = true;
                                }

                            }
                        }
                    }
                }
            }
        }
    }

}

void LengthSegmentation::segmentBySpecificLength()
{
    m_cellAnalyzed.clear();
    //Reinicializa o vetor das células analizadas
    m_cellAnalyzed.resize(m_strSeg->getRows() * m_strSeg->getCols());
    int SegmentNumber = 1;
    short xAnterior = 0;
    short yAnterior = 0;
    short row = 0;
    short col = 0;
    float yLat = 0;
    float xLon = 0;
    float totalLength = 0;

    //Faz da maior área para a menor
    for (auto nJunction = m_numberOfJunctionCells; nJunction >= 0; nJunction--)
    {
        totalLength = 0;
        col = m_junctionCells[nJunction]->x;
        row = m_junctionCells[nJunction]->y;

        //Se a célula já foi analisada, passa para a próxima junção
        if (m_cellAnalyzed[row * m_strSeg->getCols() + col] == true)
        {
            continue;
        }

        //Faça até chegar na cabeceira do rio mais longo
        while(true)
        {
            xAnterior = col;
            yAnterior = row;

            m_cellAnalyzed[row * m_strSeg->getCols() + col] = true;
            //enumera o trecho segmentado
            m_strSeg->setData(row, col, SegmentNumber);

            //Move em direção à célula à montante com maior área
            moveToUpstream(yAnterior, xAnterior, row, col, false);

            //Significa que chegou na célula de cabeceira
            if (row == yAnterior && col == xAnterior)
            {
                //Faça até atingir o limite de extensão
                while (totalLength < m_minLength)
                {
                    //Move em direção à célula à montante com maior área, quebrando a barreira do stream definition
                    moveToUpstream(yAnterior, xAnterior, row, col, true);

                    //Significa que chegou na célula mais remota da bacia, vai para a próxima junção
                    if (row == yAnterior && col == xAnterior)
                    {
                        break;
                    }

                    //Marca a célula como analisada
                    m_cellAnalyzed[row * m_strSeg->getCols() + col] = true;
                    //enumera o trecho segmentado de acordo com o trecho anterior
                    m_strSeg->setData(row, col, m_strSeg->getData(yAnterior, xAnterior));

                    //Incrementa o comprimento acumulado
                    totalLength += (SpatialAnalyst::cellLength(yLat, xLon, (yAnterior - row), (xAnterior - col), m_flowDirection->getCellSize())) * 1000;

                    //Atribui as novas posições
                    yAnterior = row;
                    xAnterior = col;
                }

                //Indica que deve ser incrementado o número de segmentos
                SegmentNumber += 1;
                //Sai fora do loop geral
                break;
            }

            //Indica que a célula já foi analisada
            m_cellAnalyzed[row * m_strSeg->getCols() + col] = true;
            //enumera o trecho segmentado
            m_strSeg->setData(row, col, SegmentNumber);

            //Pega as coordenadas do ponto
            yLat = (float)(m_flowDirection->getYOrigin() + (m_flowDirection->getRows() - 1 - row) * m_flowDirection->getCellSize() + (m_flowDirection->getCellSize() / 2));
            xLon = (float)(m_flowDirection->getXOrigin() + (col * m_flowDirection->getCellSize()) + (m_flowDirection->getCellSize() / 2));

            //Incrementa o comprimento acumulado
            totalLength += (SpatialAnalyst::cellLength(yLat, xLon, (yAnterior - row), (xAnterior - col), m_flowDirection->getCellSize())) * 1000;
            //Se comprimento acumulado for maior do que o limiar estabelecido
            if (totalLength > m_minLength)
            {
                //Aumenta a numeração dos segmentos
                SegmentNumber += 1;
                //Reinicia o comprimento acumulado
                totalLength = 0;
            }

        }
    }
}

void LengthSegmentation::moveToUpstream(short Yc, short Xc, short &rowUpstr, short &colUpstr, bool ForceHeadwater)
{

    short xi = 0;
    short yi = 0;
    float MaxUpstreamArea = 0;

    rowUpstr = Yc;
    colUpstr = Xc;

    for (auto y = -1; y <= 1; y++)
    {
        yi = (Yc + y);
        for (auto x = -1; x <= 1; x++)
        {
            xi = (Xc + x);

            if (x != 0 || y != 0) //Evita a análise da própria célula (nó central)
            {
                //Evita sair dos limites do raster
                if (xi >= 0 && yi >= 0 && xi < m_flowDirection->getCols() && yi < m_flowDirection->getRows())
                {

                    if (m_strSeg->getData(yi, xi) > 0 || ForceHeadwater == true) //Se estiver em cima da rede de drenagem
                    {
                        if (HeuristicSinkRemovalUtil::relativeIncipientFlowDirection(xi, Xc, yi, Yc) == m_flowDirection->getData(yi, xi))
                        {
                            //Verifica qual a célula com a maior área contribuinte
                            if (m_flowAcc->getData(yi, xi) > MaxUpstreamArea)
                            {
                                MaxUpstreamArea = m_flowAcc->getData(yi, xi);
                                rowUpstr = yi;
                                colUpstr = xi;
                            }
                        }
                    }
                }
            }
        }
    }
}

void LengthSegmentation::doubleSegmentByLength()
{
    m_cellAnalyzed.clear();
    // Reinicializa o vetor das células analizadas
    m_cellAnalyzed.resize(m_strSeg->getRows() * m_strSeg->getCols());
    int SegmentNumber = 1;
    int VariableSegmentNumber = 0;
    short xAnterior = 0;
    short yAnterior = 0;
    short row = 0;
    short col = 0;
    float yLat = 0;
    float xLon = 0;
    float totalLength = 0;

    //Faz da maior área para a menor
    for (auto nJunction = m_numberOfJunctionCells; nJunction >= 0; nJunction--)
    {
        totalLength = 0;
        col = m_junctionCells[nJunction]->x;
        row = m_junctionCells[nJunction]->y;

        //Caso não tiver sido atribuído previamente o valor do segmento para a célula em questão
        if (m_junctionCells[nJunction]->SegmentValue == 0)
        {
            m_junctionCells[nJunction]->SegmentValue = SegmentNumber;
            VariableSegmentNumber = SegmentNumber;
        }
        else
        {
            totalLength = m_junctionCells[nJunction]->ActualLength;
            VariableSegmentNumber = FloatingPointToInteger::ToInt32(m_junctionCells[nJunction]->SegmentValue);
        }

        if (_CellAnalyzed(row, col) == true) //Se a célula já foi analisada, passa para a próxima junção
        {
            goto nextjunction;
        }

        do //Faça até chegar na cabeceira do rio mais longo
        {
            xAnterior = col;
            yAnterior = row;

            _CellAnalyzed(row, col) = true;
            m_strSeg->getData(row, col) = VariableSegmentNumber; //enumera o trecho segmentado

            moveToUpstreamDoubleSegmented(yAnterior, xAnterior, row, col, false, totalLength, VariableSegmentNumber); //Move em direção à célula à montante com maior área

            if (row == yAnterior && col == xAnterior) //Significa que chegou na célula de cabeceira
            {

                while (totalLength < m_minLength) //Faça até atingir o limite de extensão
                {
                    moveToUpstreamDoubleSegmented(yAnterior, xAnterior, row, col, true, totalLength, VariableSegmentNumber); //Move em direção à célula à montante com maior área, quebrando a barreira do stream definition

                    if (row == yAnterior && col == xAnterior) //Significa que chegou na célula mais remota da bacia, vai para a próxima junção
                    {
                        break;
                    }

                    _CellAnalyzed(row, col) = true; //Marca a célula como analisada
                    m_strSeg->getData(row, col) = m_strSeg->getData(yAnterior, xAnterior); //enumera o trecho segmentado de acordo com o trecho anterior

                    totalLength += (SpatialAnalyst::CellLength(yLat, xLon, (yAnterior - row), (xAnterior - col), m_flowDirection->Cellsize)) * 1000; //Incrementa o comprimento acumulado

                    //Atribui as novas posições
                    yAnterior = row;
                    xAnterior = col;
                }

                SegmentNumber += 1; //Indica que deve ser incrementado o número de segmentos
                VariableSegmentNumber = SegmentNumber;
                break; //Sai fora do loop geral

            }

            _CellAnalyzed(row, col) = true; //Indica que a célula já foi analisada
            m_strSeg->getData(row, col) = VariableSegmentNumber; //enumera o trecho segmentado

            //Pega as coordanadas do ponto
            yLat = (float)(m_flowDirection->YllCorner + (m_flowDirection->getRows() - 1 - row) * m_flowDirection->Cellsize + (m_flowDirection->Cellsize / 2));
            xLon = (float)(m_flowDirection->XllCorner + (col * m_flowDirection->Cellsize) + (m_flowDirection->Cellsize / 2));

            totalLength += (SpatialAnalyst::CellLength(yLat, xLon, (yAnterior - row), (xAnterior - col), m_flowDirection->Cellsize)) * 1000; //Incrementa o comprimento acumulado
            if (totalLength > m_minLength) //Se comprimento acumulado for maior do que o limiar estabelecido
            {
                SegmentNumber += 1; //Aumenta a numeração dos segmentos
                VariableSegmentNumber = SegmentNumber;
                totalLength = 0; //Reinicia o comprimento acumulado
            }

        } while (true);
    nextJunction: ;

    }

}

void LengthSegmentation::moveToUpstreamDoubleSegmented(short Yc, short Xc, short &rowUpstr, short &colUpstr, bool ForceHeadwater, float ActualLength, int segmentValue)
{

    short xi = 0;
    short yi = 0;
    short nPointCell = 0;
    float MaxUpstreamArea = 0;
    long long cellNum = 0;
    bool restartVerification = false;
    rowUpstr = Yc;
    colUpstr = Xc;

    restartPosition:
    nPointCell = 0;

    for (auto y = -1; y <= 1; y++)
    {
        yi = (Yc + y);
        for (auto x = -1; x <= 1; x++)
        {
            xi = (Xc + x);

            if (x != 0 || y != 0) //Evita a análise da própria célula (nó central)
            {
                if (xi >= 0 && yi >= 0 && xi < m_flowDirection->getCols() && yi < m_flowDirection->getRows()) //Evita sair dos limites do raster
                {

                    if (m_strSeg->getData(yi, xi) > 0 || ForceHeadwater == true) //Se estiver em cima da rede de drenagem
                    {

                        if (FlowDirection::relativeIncipientFlowDirection(xi, Xc, yi, Yc) == m_flowDirection->getData(yi, xi))
                        {

                            nPointCell += 1;

                            if (restartVerification == true) //Se identificou que existe uma junção, atribui os dados para as células de exutório
                            {
                                cellNum = findCellPosition(yi, xi);
                                if (!(cellNum < 0))
                                {
                                    m_junctionCells[cellNum]->ActualLength = ActualLength;
                                    m_junctionCells[cellNum]->SegmentValue = segmentValue;
                                }
                            }

                            //Verifica qual a célula com a maior área contribuinte
                            if (m_flowAcc->getData(yi, xi) > MaxUpstreamArea)
                            {
                                MaxUpstreamArea = m_flowAcc->getData(yi, xi);
                                rowUpstr = yi;
                                colUpstr = xi;
                            }
                        }
                    }
                }
            }
        }
    }

    if (nPointCell > 1 && restartVerification == false)
    {
        restartVerification = true;
        goto restartPosition;
    }

}

void LengthSegmentation::doubleSegmentedPosProcessing()
{

//VB TO C++ CONVERTER TODO TASK: Native C++ has no direct way to resize an array:
    ReDim _CellAnalyzed(m_strSeg->getRows(), m_strSeg->getCols()); //Reinicializa o vetor das células analizadas
    short xAnterior = 0;
    short yAnterior = 0;
    short row = 0;
    short col = 0;
    bool MovedUpJunction = false;
    int SegmentCode = 0;
    int PreviousSegmentCode = 0;

    for (auto nJunction = (m_numberOfJunctionCells - 1); nJunction >= 0; nJunction--) //Faz da maior área para a menor, ignorando o exutório final
    {

        col = m_junctionCells[nJunction]->x;
        row = m_junctionCells[nJunction]->y;

        xAnterior = col;
        yAnterior = row;

        if (m_strSeg->getData(row, col) == m_strSeg->NoDataValue) //Se a célula já foi analisada e removida da drenagem principal, passa para a próxima junção
        {
            goto nextjunction;
        }
        FlowDirection::moveToFlowDirection(m_flowDirection->getData(row, col), row, col); //Anda uma célula para jusante para alcançar a célula de junção (com exceção do exutório da bacia '0')

        SegmentCode = m_strSeg->getData(row, col); //Identifica o número do segmento atual

        if (SegmentCode == m_strSeg->NoDataValue) //Significa que o trecho a jusante já foi deletado. Deverá ser deletado o trecho atual
        {
            row = yAnterior; //Volta para a posição anterior
            col = xAnterior;
            SegmentCode = m_strSeg->getData(yAnterior, xAnterior);
            PreviousSegmentCode = SegmentCode;
            goto EraseCells; //Vai direto para a exclusão do trecho
        }
        else
        {
            xAnterior = col;
            yAnterior = row;
        }

        PreviousSegmentCode = SegmentCode; //Assume o número do segmento anterior igual ao atual

        MovedUpJunction = false; //Parte inicialmente do pressuposto que não passou da junção
        moveToUpstreamMinArea(yAnterior, xAnterior, row, col, MovedUpJunction, false); //Sobe em direção à área mínima de flow accumulation, na junção atual

        if (MovedUpJunction == true) //Caso passar da junção e ir em direção à menor área, começa a exclusão do trecho
        {

            do //Faça até chegar em uma junção ou trocar de código de segmentação
            {

                SegmentCode = m_strSeg->getData(row, col); //Identifica o código do segmento após a mudança de posição

                if (yAnterior == row && xAnterior == col) //Chegou na célula de cabeceira
                {
                    break;
                }
    EraseCells:
                if (PreviousSegmentCode != SegmentCode) //Se trocar o código do segmento, vai para a próxima junção
                {
                    break;
                }
                m_strSeg->getData(row, col) = m_strSeg->NoDataValue; //Apaga a definição de rede de drenagem da célula

                xAnterior = col;
                yAnterior = row;

                MovedUpJunction = false; //Parte inicialmente do pressuposto que não passou da junção
                moveToUpstreamMinArea(yAnterior, xAnterior, row, col, MovedUpJunction, false); //Sobe em direção à área mínima de flow accumulation

                if (MovedUpJunction == true) //Chegou em uma nova junção, sai do loop
                {
                    MovedUpJunction = false;
                    break;
                }
            } while (true);
        }

    nextjunction: ;
    }

}

void LengthSegmentation::moveToUpstreamMinArea(short Yc, short Xc, short &rowUpstr, short &colUpstr, bool &AfterJunction, bool markOutlets)
{

    short xi = 0;
    short yi = 0;
    float MinUpstreamArea = std::numeric_limits<float>::max(); //Em direção à menor área de drenagem
    short CounterPointingCells = 0; //Conta quantas células apontaram para a célula atual

    AfterJunction = false;

    rowUpstr = Yc;
    colUpstr = Xc;

    for (auto y = -1; y <= 1; y++)
    {
        yi = (Yc + y);
        for (auto x = -1; x <= 1; x++)
        {
            xi = (Xc + x);

            if (x != 0 || y != 0) //Evita a análise da própria célula (nó central)
            {
                if (xi >= 0 && yi >= 0 && xi < m_flowDirection->getCols() && yi < m_flowDirection->getRows()) //Evita sair dos limites do raster
                {

                    if (m_strSeg->getData(yi, xi) > 0) //Se estiver em cima da rede de drenagem
                    {

                        if (FlowDirection::relativeIncipientFlowDirection(xi, Xc, yi, Yc) == m_flowDirection->getData(yi, xi))
                        {

                            CounterPointingCells += 1;
                            if (markOutlets == true) //Marca o exutório para a respectiva drenagem ser removida posteriormente
                            {
                                m_cellAnalyzed(yi, xi) = true;
                            }

                            //Verifica qual a célula com a menor área contribuinte
                            if (m_flowAcc->getData(yi, xi) < MinUpstreamArea)
                            {
                                MinUpstreamArea = m_flowAcc->getData(yi, xi);
                                rowUpstr = yi;
                                colUpstr = xi;
                            }
                        }
                    }
                }
            }
        }
    }

    if (CounterPointingCells > 1) //Avisa que chegou em uma junção e moveu para a célula de menor área
    {
        AfterJunction = true;
    }

}

long long LengthSegmentation::findCellPosition(short y, short x)
{

    for (auto i = 0; i < m_numberOfJunctionCells; i++)
    {
        if (m_junctionCells[i]->x == x && m_junctionCells[i]->y == y)
        {
            return i;
        }
    }

    return -1; //Caso não encontrar a célula no meio das demais
    //Throw New Exception("Junction cells were not found!")

}

void LengthSegmentation::writeSegmentedStreams(const std::QString &arquivo)
{
    m_strSeg->WriteData(arquivo);
}
}
}
