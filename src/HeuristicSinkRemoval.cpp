#include "continental/hydrotools/Cell.h"
#include "continental/hydrotools/HeuristicSinkRemoval.h"
#include "continental/hydrotools/HeuristicSinkRemovalUtil.h"
#include "continental/hydrotools/Cell.h"
#include "continental/hydrotools/HeuristicCell.h"
#include <sstream>

using namespace std;
using namespace continental::datamanagement;

namespace continental
{
namespace hydrotools
{

int HeuristicSinkRemoval::getWindowSize() const
{
    return m_windowSize;
}

void HeuristicSinkRemoval::setWindowSize(int value)
{
    m_windowSize = value;
}

HeuristicSinkRemoval::ProcessingMode HeuristicSinkRemoval::getProcessMode() const
{
    return m_algorithmMode;
}

void HeuristicSinkRemoval::setProcessMode(ProcessingMode value)
{
    m_algorithmMode = value;
}

shared_ptr<Raster<short>> HeuristicSinkRemoval::getDem() const
{
    return m_dem;
}

void HeuristicSinkRemoval::setDem(shared_ptr<Raster<short>> dem)
{
    m_dem = dem;
    m_flowDirection = make_shared<Raster<short>>(m_dem->getRows(), m_dem->getCols(), m_dem->getXOrigin(), m_dem->getYOrigin(), m_dem->getCellSize(), m_dem->getNoDataValue());
    m_forcedOutlets.clear();
    m_forcedOutlets.resize(m_dem->getRows() * m_dem->getCols());
}

shared_ptr<Raster<short>> HeuristicSinkRemoval::getFlowDirection() const
{
    return m_flowDirection;
}

HeuristicSinkRemoval::HeuristicSinkRemoval(size_t maxOpenList, size_t maxClosedList, float weightFunctionCost, HeuristicSinkRemoval::ProcessingMode processingAlgorithm)
{
    m_maxClosedList = maxClosedList;
    m_maxOpenList = maxOpenList;
    m_weightFunctionCost = weightFunctionCost;
    m_algorithmMode = processingAlgorithm;
}

HeuristicSinkRemoval::~HeuristicSinkRemoval()
{

}

void HeuristicSinkRemoval::removeSinks()
{
    // Flag que indica se ocorreu algum erro no processo: inicialmente é dado como VERDADEIRO
    error = true;

    m_array.clear();
    m_array.resize(m_dem->getRows() * m_dem->getCols());

    // Calcula o Flow Direction na borda
    flowDirectionAtBounds();
    // Calcula o Flow Direction de toda a matriz
    calculateFlowDirection();
    // Realiza uma operação de Fill, atribuindo a cota do vizinho com elevação mais próxima
    fillDepressions(0);

    // Lista de nós, candidatos a serem selecionados para um possível trajeto de mínimo custo
    m_openList.clear();
    m_openList.resize(m_maxOpenList);

    m_openListPosition.clear();
    m_openListPosition.resize(m_maxOpenList);

    m_openListBoolean.clear();
    m_openListBoolean.resize(m_dem->getRows() * m_dem->getCols());

    // Lista de nós que já foram selecionados e checados
    m_closedList.clear();
    m_closedList.resize(m_maxClosedList);

    m_closedListPosition.clear();
    m_closedListPosition.resize(m_maxClosedList);

    m_closedListBoolean.clear();
    m_closedListBoolean.resize(m_dem->getRows() * m_dem->getCols());

    // Matriz que indica o caminho de volta, a partir do outlet
    m_traceBackMatrix.clear();
    m_traceBackMatrix.resize(m_dem->getRows() * m_dem->getCols());

    // Remove as depressões
    removeDepressions();
    // Libera a memória
    releaseMemory();
    // Remove os ruídos do MDE final de acordo com as direções de fluxo
    adjustDemFinalElevations();

    // Se chegar até aqui, então não ocorreram erros
    error = false;
}

void HeuristicSinkRemoval::removeDepressions()
{
    // Número de elementos da closed list, que será variável
    size_t numberClosedList = 0;
    // Número de elementos da open list, que será variável
	size_t numberOpenList = 0;
	size_t numberClosedListForcedBound = 0;

    bool outletFound = false;
    size_t nDepressions = m_array.size();
    short startElevation = 0, endElevation = 0;
    size_t xPosition = 0, yPosition = 0;
			
    // Faça para todas as depressões encontradas
    for (size_t iArray = 0; iArray < nDepressions; ++iArray)
    {
        // Indica o percentual concluído
        percent = iArray / nDepressions;

        // Torna falso o encontro da saída
        outletFound = false;
        // Identifica a posição X da depressão
        xPosition = m_array[iArray]->x;
        // Identifica a posição Y da depressão
        yPosition = m_array[iArray]->y;

        // Pode ser que o Flow Direction já tenha sido calculado em uma das etapas abaixo!
        if (m_flowDirection->getData(yPosition, xPosition) == 0)
        {
            // Identifica a elevação da depressão selecionada
            startElevation = m_dem->getData(yPosition, xPosition);
            // Adiciona inicialmente a célula com a depressão na open list, para ser analisada
            addDepressionToOpenList(xPosition, yPosition, numberOpenList);
            // Move a célula da depressão, da open list para a closed list
            pushCellToClosedList(0, numberClosedList);
            // Adiciona os vizinhos da célula com depressão na open list, de forma que estas sejam candidatas à solução do problema
            addNeighboursToOpenList(yPosition, xPosition, numberOpenList);

            // Faça até que achar uma célula em que pode verter a água
            while (!outletFound)
            {
                // Calcula o value heurístico de cada uma das células da openlist
                for (size_t z = 0; z < numberOpenList; ++z)
                {
                    if (m_openList[z].get() != nullptr)
                    {
                        //Identifica o método que será utilizado para remoção de depressões
                        if (m_algorithmMode == ProcessingMode::MHS)
                        {
                            m_openList[z]->costFunction = modifiedHeuristicValue(
                                        static_cast<int>(m_openList[z]->y),
                                        static_cast<int>(m_openList[z]->x),
                                        startElevation,
                                        static_cast<short>(m_openList[z]->y - yPosition),
                                        static_cast<short>(m_openList[z]->x - xPosition),
                                        m_weightFunctionCost);
                        }
                        else if (m_algorithmMode == ProcessingMode::HS)
                        {
                            m_openList[z]->costFunction = heuristicValue(static_cast<int>(m_openList[z]->y), static_cast<int>(m_openList[z]->x), startElevation);
                        }
                        else if (m_algorithmMode == ProcessingMode::PFS)
                        {
                            m_openList[z]->costFunction = pfsValue(m_openList[z]->y, m_openList[z]->x, startElevation);
                        }

                    }
                }

                // Identifica a célula com o maior value heurístico na open list
                size_t iNextCell = enumMinHeuristicInfo(numberOpenList);
                // Elevação inicial da depressão
                endElevation = m_dem->getData(m_openList[iNextCell]->y, m_openList[iNextCell]->x);

                // Verifica se encontrou a saída para o problema
                outletFound = isOutlet(endElevation, startElevation, m_openList[iNextCell]->y, m_openList[iNextCell]->x);

                // Força a saída de água no limite mais próximo da borda, caso exceder um numero pré-determinado de células
                if (numberClosedList == m_maxClosedList)
                {
                    numberClosedListForcedBound = m_maxClosedList;
                    forceBoundOutlet(m_closedList[0]->y, m_closedList[0]->x, m_maxClosedList, numberClosedList);
                    outletFound = true;

                    //Modifica o MDE para o caminho encontrado
                    breaching(numberClosedList);

                    //Retorna o número de elementos original para resetar a lista
                    if (numberClosedListForcedBound == m_maxClosedList)
                    {
                        numberClosedList = numberClosedListForcedBound;
                        numberClosedListForcedBound = 0;
                    }

                    resetAllList(numberClosedList, numberOpenList);
                    numberClosedList = 0;
                    numberOpenList = 0;

                    continue;
                }

                if (outletFound == false)
                {
                    // Se não encontrou um ponto para verter a água, continua realizando a análise

					// Atualiza a posição da célula atual
					yPosition = m_openList[iNextCell]->y;
					xPosition = m_openList[iNextCell]->x;

                    // Move a célula para a closed list e retira a mesma da open list
                    pushCellToClosedList(iNextCell, numberClosedList);
                    // Adiciona os vizinhos da célula selecionada (de máximo value heurístico) na open list, para serem avaliados
                    addNeighboursToOpenList(yPosition, xPosition, numberOpenList);
                }
                else
                {
                    // Termina a operação, estabelece o caminho e reseta as listas para a próxima depressão
                    // Move a célula para a closed list e retira a mesma da open list
                    pushCellToClosedList(iNextCell, numberClosedList);

                    //Modifica o MDE para o caminho encontrado
                    breaching(numberClosedList);

                    //Retorna o número de elementos original para resetar a lista
                    if (numberClosedListForcedBound == m_maxClosedList)
                    {
                        numberClosedList = numberClosedListForcedBound;
                        numberClosedListForcedBound = 0;
                    }

                    resetAllList(numberClosedList, numberOpenList);
                    numberClosedList = 0;
                    numberOpenList = 0;
                }
            }
        }
    }

}

void HeuristicSinkRemoval::addDepressionToOpenList(size_t xc, size_t yc, size_t &numberElements)
{
    // Coloca a célula na open list
    m_openList[numberElements] = make_unique<HeuristicCell>(yc, xc);
    //Aciona o Flag para indicar que a célula está na open list
    m_openListBoolean[yc * m_dem->getCols() + xc] = true;
    m_openList[numberElements]->relParentX = 0;
    m_openList[numberElements]->relParentY = 0;
    // Identifica a posição da célula na matriz (na forma de vetor)
    m_openListPosition[numberElements] = yc * m_dem->getCols() + xc;

    numberElements += 1;
}

void HeuristicSinkRemoval::addNeighboursToOpenList(size_t yc, size_t xc, size_t &numberElementos)
{
    //Yc = Y da célula central
    //Xc = X da célula central
    int posX = 0, posY = 0;

    //Utiliza uma matriz '3 x 3' em torno da célula central, identificando as células vizinhas e fazendo um somatório das elevações
    for (int y = -1; y <= 1; ++y)
    {
        posY = static_cast<int>(yc) + y;
        for (int x = -1; x <= 1; ++x)
        {
            posX = static_cast<int>(xc) + x;

            // Evita que o vizinho saia fora dos limites da grade
            if (posX < 0 || posY < 0 || posX >= static_cast<int>(m_dem->getCols()) || posY >= static_cast<int>(m_dem->getRows()))
			{
				continue;
			}

            // Somente adiciona se não estiver nem na open list e nem na closed list
            size_t position = static_cast<size_t>(posY) * m_dem->getCols() + static_cast<size_t>(posX);
			if (m_closedListBoolean[position] || m_openListBoolean[position])
			{
				continue;
			}

            // Somente adiciona se não for o ponto central
			if (y == 0 && x == 0)
			{
				continue;
			}

            m_openList[numberElementos] = make_unique<HeuristicCell>(static_cast<size_t>(posY), static_cast<size_t>(posX));
            // Estabelece pointers relativos para o o nó de origem
            m_openList[numberElementos]->relParentX = static_cast<int>(xc) - posX;
            m_openList[numberElementos]->relParentY = static_cast<int>(yc) - posY;
            // Aciona o flag indicando que a célula está na open list
            m_openListBoolean[position] = true;
            // Indica a posição do ponto da open list
            m_openListPosition[numberElementos] = static_cast<size_t>(posY) * m_dem->getCols() + static_cast<size_t>(posX);
            numberElementos += 1;
        }
    }
}

void HeuristicSinkRemoval::adjustDemFinalElevations()
{
    std::vector<bool> checkedNodes(m_dem->getRows() * m_dem->getCols(), false);

    int rows = static_cast<int>(m_dem->getRows()), cols = static_cast<int>(m_dem->getCols());

    // Fazer para toda a grade
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            auto row = i;
            auto column = j;

            // Armazena a elevação da célula atual
            short previousElevation = m_dem->getData(row, column);

            size_t position = static_cast<size_t>(row * cols + column);

            // Se a célula atual não tiver sido checada ainda
            if (checkedNodes[position])
            {
                continue;
            }
            while (true)
            {
                // Move para a célula de jusante, de acordo com o flow direction
                HeuristicSinkRemovalUtil::moveToFlowDirection(m_flowDirection->getData(row, column), row, column);
                position = static_cast<size_t>(row * cols + column);
                // Evita sair fora dos limites
                if (row > (rows - 1) || column > (cols - 1))
                {
                    break;
                }

                // Se a célula atual (de jusante) já tiver sido checada, sai do loop
                if (checkedNodes[position] == true)
                {
                    break;
                }

                // Armazena a elevação da célula a jusante
                short nextElevation = m_dem->getData(row, column);

                // Se a célula a jusante tiver uma cota superior à anterior, iguala a da anterior
                if (nextElevation > previousElevation)
                {
                    m_dem->setData(static_cast<size_t>(row), static_cast<size_t>(column), previousElevation);
                }
                // Assume a célula como checada
                checkedNodes[position] = true;
                // A nova elevação da célula
                previousElevation = nextElevation;

                if (row < 0 || column < 0 || row > (rows - 1) || column > cols)
                {
                    break;
                }
            }
        }
    }
}

void HeuristicSinkRemoval::breaching(size_t closedListCount)
{
    //identifica as elevações inicial e final da pathlist
    short initElevation = m_dem->getData(m_closedList[0]->y, m_closedList[0]->x);
    short finalElevation = m_dem->getData(m_closedList[closedListCount - 1]->y, m_closedList[closedListCount - 1]->x);

    //Evita que o programa utilize o value do NoDATA como elevação final, o que pode comprometer o resultado
    if (finalElevation == m_dem->getNoDataValue())
    {
        finalElevation = initElevation;
    }

    if (finalElevation > initElevation)
    {
        // Pode ocorrer em situações de limite do MDE, nesse caso, força o decréscimo na cota
        finalElevation = initElevation;
        m_dem->setData(m_closedList[closedListCount - 1]->y, m_closedList[closedListCount - 1]->x, finalElevation);
    }

    size_t numberCells = 0, xParent = 0, yParent = 0;
    bool foundStartNode = false;
    size_t enumerator = closedListCount - 1;

    // Identifica o nº de células no caminho do outlet até a depressão
    while (!foundStartNode)
    {
        // Acha a coordenada do parent node
        xParent = m_closedList[enumerator]->x + static_cast<size_t>(m_closedList[enumerator]->relParentX);
        yParent = m_closedList[enumerator]->y + static_cast<size_t>(m_closedList[enumerator]->relParentY);

        if (m_closedList[enumerator]->relParentX == 0 && m_closedList[enumerator]->relParentY == 0)
        {
            foundStartNode = true;
        }

        // Identifica o enumerador do nó de coordenada x,y
        enumerator = m_traceBackMatrix[yParent * m_dem->getCols() + xParent];
        numberCells += 1;
    }

    // Calcula o incremento para escavação entre a elevação do outlet e da depressão
    float incremental = static_cast<float>(initElevation - finalElevation) / static_cast<float>(numberCells);

    //Identifica o flow Direction para cada uma das células do pathlist e ajusta as respectivas cotas do MDE
    enumerator = closedListCount - 1;

    //A primeira célula eu não posso modificar, pois é o outlet; a ´´ultima célula, que é o ponto de saída, também não modifico
    for (size_t i = 1; i < numberCells; ++i)
    {
        //Acha a coordenada do parent node
        xParent = m_closedList[enumerator]->x + static_cast<size_t>(m_closedList[enumerator]->relParentX);
        yParent = m_closedList[enumerator]->y + static_cast<size_t>(m_closedList[enumerator]->relParentY);
        //Atribuo o novo flow direction
        m_flowDirection->setData(yParent, xParent, HeuristicSinkRemovalUtil::relativeIncipientFlowDirection(static_cast<int>(xParent), static_cast<int>(m_closedList[enumerator]->x), static_cast<int>(yParent), static_cast<int>(m_closedList[enumerator]->y)));

        // Só atribuo a cota se ela for menor do que a existente
        auto value = static_cast<short>(finalElevation + std::nearbyint(incremental * i));
        if (m_dem->getData(yParent, xParent) > value)
        {
            // Atribuo a nova cota
            m_dem->setData(yParent, xParent, value);
        }

        // Identifica o enumerador do nó de coordenada x,y
        enumerator = m_traceBackMatrix[yParent * m_dem->getCols() + xParent];
    }
}

size_t HeuristicSinkRemoval::enumMinHeuristicInfo(size_t nOpenlist)
{

    float value = 99999;
    size_t enumerator = 0;
    size_t limit = nOpenlist;
    for (size_t i = 0; i < limit; ++i)
    {
        if (m_openList[i] == nullptr)
        {
            continue;
        }

        auto costFunction = m_openList[i]->costFunction;
        if (costFunction >= value)
        {
            continue;
        }

        value = costFunction;
        enumerator = i;
    }

    return enumerator;

}

void HeuristicSinkRemoval::fillDepressions(int differenceHeight)
{
    size_t xc = 0, yc = 0, nDepressions = m_array.size();
    short elevation = 0;
    short lowerBound = 0;

    for (size_t i = 0; i < nDepressions; ++i)
    {
        yc = m_array[i]->y;
        xc = m_array[i]->x;

        //Assume a elevação da célula '//MODIFICADO EM 21/11/2013
        elevation = m_dem->getData(yc, xc);
        lowerBound = 9999;

        //Verifica qual é o vizinho com menor elevação
        for (int yi = -1; yi <= 1; ++yi)
        {
            for (int xi = -1; xi <= 1; ++xi)
            {
                //exclui a verificação para o ponto central
				if (yi == 0 && xi == 0)
				{
					continue;
				}

                auto y = static_cast<size_t>(static_cast<int>(yc) + yi);
                auto x = static_cast<size_t>(static_cast<int>(xc) + xi);

                //Não calcula a elevação for NODATA
                if (m_dem->getData(y, x) == m_dem->getNoDataValue())
				{
					continue;
				}

                if (m_dem->getData(y, x) >= lowerBound)
				{
					continue;
				}

                lowerBound = m_dem->getData(y, x);
            }
        }

        //Se a diferença entre a elevação de um vizinho e a elevação da depressão é maior do que um limite pré definido, aumenta a cota até este ponto
        if ((lowerBound - elevation) >= differenceHeight)
        {
            m_dem->setData(yc, xc, lowerBound);
        }
    }

}

void HeuristicSinkRemoval::forceBoundOutlet(size_t yc, size_t xc, size_t maxClosedListElements, size_t &numberActualClosedListElements)
{
    int elements = -1;

    // Verifica, dentre todas as células da closed list, a primeira célula que for de borda
    for (size_t w = 0; w < maxClosedListElements; ++w)
    {
        if (m_closedList[w]->x == 0 || m_closedList[w]->y == 0 || m_closedList[w]->x == (m_dem->getCols() - 1) || m_closedList[w]->y == (m_dem->getRows() - 1))
        {
            elements = static_cast<int>(w) + 1;
            break;
        }
    }

    // Caso não houver nenhuma célula de borda, dispara o erro
    if (elements == -1)
    {
        stringstream buffer("The maximum number of elements from the closed list was not enough to find the river section at X,Y = (");
        buffer << xc << "," << yc << ").";
        throw std::runtime_error(buffer.str());
    }

    numberActualClosedListElements = static_cast<size_t>(elements);
}

float HeuristicSinkRemoval::heuristicValue(int yc, int xc, short es)
{
    //PosY = posição Y da célula a ser somada
    //PosX = posição X da célula a ser somada
    //Ei = value da elevação na célula de indice 'i'
    //Es = value da elevação na célula inicial (starting node)

    int numberCells = 0;
    short ei = m_dem->getData(yc, xc);
    int posX = 0, posY = 0;

    // Divide dimensão por 2 e arredonda para o inteiro inferior; se n = 5, vai de -2 até 2
    int n = static_cast<int>(std::floor(m_windowSize / 2.0));
    size_t sum = 0;

    // value da função G(i) -> diferença de cota entre o nó inicial e o de índice 'i'
    short gi = ei - es;
    float hi = 0;

    // Utiliza uma matriz 'n x n' em torno da célula central (porém incluindo a mesma), identificando as células vizinhas e fazendo um somatório das elevações
    for (int y = -n; y <= n; ++y)
    {
        posY = yc + y;
        for (int x = -n; x <= n; ++x)
        {
            posX = xc + x;

            // Evita que o vizinho saia fora dos limites da grade
            if (posX >= 0 && posY >= 0 && posX < static_cast<int>(m_dem->getCols()) && posY < static_cast<int>(m_dem->getRows()))
            {
                sum += static_cast<size_t>(m_dem->getData(static_cast<size_t>(yc + y), static_cast<size_t>(xc + x)));
                numberCells += 1;
            }
        }
    }

    // value da função H(i), evita divisão por 0 (modificado em relação à original)
    if (numberCells != 0)
    {
        hi = sum / static_cast<float>(numberCells);
    }

    // value da função heurística F(i) = H(i) + G(i)
    return gi + hi;
}

bool HeuristicSinkRemoval::isOutlet(short ei, short es, size_t posY, size_t posX)
{
    // Se chegar no NODATA, retorna true
    if (ei == m_dem->getNoDataValue())
    {
        return true;
    }
    // Se a diferença de cota do ponto final for menor ou igual a 2 e estiver na borda do MDE, aceita como exutório
    else if (ei < es)
    {
        return true;
    }
    else if ((ei - es) <= 2 && (posX == 0 || posY == 0 || (posX == m_dem->getCols() - 1) || (posY == m_dem->getRows() - 1)))
    {
        return true;
    }

    return false;
}

float HeuristicSinkRemoval::modifiedHeuristicValue(int yc, int xc, short es, short relPosY, short relPosX, float wg)
{
    //ei = value da elevação na célula de indice 'i'
    //es = value da elevação na célula inicial (starting node)

    int numberCells = 0;
    short ei = m_dem->getData(static_cast<size_t>(yc), static_cast<size_t>(xc));
    int posX = 0;
    int posY = 0;
    short dy1 = 0;
    short dy2 = 0;
    short dx1 = 0;
    short dx2 = 0;

    double sum = 0;

    //value da função G(i) -> diferença de cota entre o nó inicial e o de índice 'i'
    short gi = ei - es;
    short hi = 0;

    //Identifica os limites da variação em y
    switch (relPosY)
    {
        case -1:
            dy1 = -2;
            dy2 = 0;
            break;
        case 0:
            dy1 = -1;
            dy2 = 1;
            break;
        case 1:
            dy1 = 0;
            dy2 = 2;
            break;
    }

    //Identifica os limites da variação em x
    switch (relPosX)
    {
        case -1:
            dx1 = -2;
            dx2 = 0;
            break;
        case 0:
            dx1 = -1;
            dx2 = 1;
            break;
        case 1:
            dx1 = 0;
            dx2 = 2;
            break;
    }

    //Utiliza uma matriz 'n x n' nos limites das células em torno da célula central, fazendo um somatório das elevações
    for (int y = dy1; y <= dy2; ++y)
    {
        posY = static_cast<int>(yc) + y;
        for (int x = dx1; x <= dx2; ++x)
        {
            posX = static_cast<int>(xc) + x;

            // Evita que o vizinho saia fora dos limites da grade
            if (posX >= 0 && posY >= 0 && posX < static_cast<int>(m_dem->getCols()) && posY < static_cast<int>(m_dem->getRows()))
            {
                sum += m_dem->getData(static_cast<size_t>(yc + y), static_cast<size_t>(xc + x));
                numberCells += 1;
            }
        }
    }

    //value da função H(i), evita divisão por 0 (modificado em relação à original)
    if (numberCells != 0)
    {
        hi = static_cast<short>(nearbyint(sum / static_cast<double>(numberCells)));
    }

    //value da função heurística F(i) = H(i) + G(i) * peso
    return (wg * gi) + hi;

}

float HeuristicSinkRemoval::pfsValue(size_t yc, size_t xc, short es)
{
    // ei = value da elevação na célula de indice 'i'
    // es = value da elevação na célula inicial (starting node)

    short ei = m_dem->getData(yc, xc);
    // value da função PFS(i) -> diferença de cota entre o nó inicial e o de índice 'i'
    return ei - es;
}

void HeuristicSinkRemoval::pushCellToClosedList(size_t enumCell, size_t &numberClosed)
{
    size_t position = m_openList[enumCell]->y * m_dem->getCols() + m_openList[enumCell]->x;

    // Aciona o Flag para indicar que a célula está na closed list
    m_closedListBoolean[position] = true;
    // Coloca a célula na closed list
    m_closedList[numberClosed] = make_unique<HeuristicCell>(m_openList[enumCell]->y, m_openList[enumCell]->x);
    m_closedList[numberClosed]->relParentX = m_openList[enumCell]->relParentX;
    m_closedList[numberClosed]->relParentY = m_openList[enumCell]->relParentY;

    //Identifica a posição da célula na matriz (na forma de vetor)
    m_closedListPosition[numberClosed] = position;
    //guarda a posição da célula para conhecer o caminho de volta
    m_traceBackMatrix[position] = numberClosed;

    //Retira o flag para indicar que a célula não está mais na open list
    m_openListBoolean[position] = false;
    // Retira a célula da open list
    m_openList[enumCell] = nullptr;

    numberClosed += 1;
}

void HeuristicSinkRemoval::releaseMemory()
{
    m_openList.clear();
    m_closedList.clear();
    m_array.clear();
    m_traceBackMatrix.clear();
    m_closedListBoolean.clear();
    m_closedListPosition.clear();
    m_openListBoolean.clear();
    m_openListPosition.clear();
}

void HeuristicSinkRemoval::resetAllList(size_t nClosedList, size_t nOpenList)
{
    size_t x = 0, y = 0;

    for (size_t i = 0; i < nClosedList; ++i)
    {
        // Identifica a linha
        y = static_cast<size_t>(std::floor(static_cast<double>(m_closedListPosition[i]) / static_cast<double>(m_dem->getCols())));
        // Identifica a Coluna
        x = m_closedListPosition[i] - (y * m_dem->getCols());
        // Retira da Closed List
        m_closedListBoolean[y * m_dem->getCols() + x] = false;
        // Reinicia o value da Matriz de volta
        m_traceBackMatrix[y * m_dem->getCols() + x] = 0;
    }

    for (size_t i = 0; i < nOpenList; ++i)
    {
        // Identifica a linha
        y = static_cast<size_t>(std::floor(static_cast<double>(m_openListPosition[i]) / static_cast<double>(m_dem->getCols())));
        // Identifica a Coluna
        x = m_openListPosition[i] - (y * m_dem->getCols());
        // Retira da Open List
        m_openListBoolean[y * m_dem->getCols() + x] = false;
    }

}

void HeuristicSinkRemoval::calculateFlowDirection()
{
    size_t countDepression = 0;
    bool fdFound = false;

    // Calcula para toda a grade as direções de fluxo
    size_t tempVar = m_dem->getRows() - 2;
    size_t tempVar2 = m_dem->getCols() - 2;
    for (size_t y = 1; y <= tempVar; ++y)
    {
        for (size_t x = 1; x <= tempVar2; ++x)
        {
            m_flowDirection->setData(y, x, incipientFlowDirection(x, y));
            // Se for = 0, verifica se existe uma célula com igual cota para onde possa verter
            if (m_flowDirection->getData(y, x) == 0)
            {
                verifyFlowDirAtBounds(x, y, fdFound);
                // Se não encontrar célula para verter água, identifica uma depressão
                if (!fdFound)
                {
                    m_array[countDepression] = make_unique<Cell>(y, x);
                    countDepression += 1;
                }

                fdFound = false;
            }
        }
    }

    // Caso não houverem depressões, a lista não existirá
	if (countDepression == 0)
	{
		m_array.clear();
	}
	else
	{
		// Caso houverem depressões, redimensiona o vetor para o tamanho de células encontradas
		m_array.resize(countDepression);
	}
}

void HeuristicSinkRemoval::verifyFlowDirAtBounds(size_t xc, size_t yc, bool &validated)
{
    //na openlist:,         'direções de apontamento para a célula central
    //For y = -1 To 1       2  4  8
    //posY = (Yc + y)       1    x   16
    //For x = -1 To 1       128   64  32
    //posX = (xc + x)

    validated = false;
    short elevation = m_dem->getData(static_cast<size_t>(yc), static_cast<size_t>(xc));
    size_t xi = 0, yi = 0;

    // Para os elementos da matriz de direções
    for (short index = 0; index <= 7; ++index)
    {
        yi = static_cast<size_t>(static_cast<int>(yc) + m_directionsY[index]);
        xi = static_cast<size_t>(static_cast<int>(xc) + m_directionsX[index]);

        // Somente se a célula possui cota igual àquela sendo analisada
        if (m_dem->getData(yi, xi) != elevation)
		{
			continue;
		}

        if (m_flowDirection->getData(yi, xi) == 0)
		{
			validated = true;
		}

        // caso tenha sido encontrado algum vizinho com direção de fluxo atribuída, manda água pra ele
        if (validated)
        {
            m_flowDirection->setData(yc, xc, HeuristicSinkRemovalUtil::relativeIncipientFlowDirection(static_cast<int>(xc), static_cast<int>(xi), static_cast<int>(yc), static_cast<int>(yi)));
            return;
        }
    }
}

void HeuristicSinkRemoval::flowDirectionAtBounds()
{
    const size_t zero = 0;

    //Define o Flow direction dos cantos
    m_flowDirection->setData(zero, zero, -9999);
    m_flowDirection->setData(zero, m_dem->getCols() - 1, -9999);
    m_flowDirection->setData(m_dem->getRows() - 1, zero, -9999);
    m_flowDirection->setData(m_dem->getRows() - 1, m_dem->getCols() - 1, -9999);

    //Atribui o flow direction para fora dos limites da grade
    size_t tempVar = m_dem->getRows() - 2;
	for (size_t y = 1; y <= tempVar; ++y)
	{
        m_flowDirection->setData(y, zero, -9999);
	}

    for (size_t y = 1; y <= tempVar; ++y)
	{
        m_flowDirection->setData(y, m_dem->getCols() - 1, -9999);
	}

    size_t tempVar3 = m_dem->getCols() - 2;
    for (size_t x = 1; x <= tempVar3; ++x)
	{
        m_flowDirection->setData(zero, x, -9999);
	}

    for (size_t x = 1; x <= tempVar3; ++x)
	{
        m_flowDirection->setData(m_dem->getRows() - 1, x, -9999);
	}
}

short HeuristicSinkRemoval::incipientFlowDirection(size_t x, size_t y)
{
    auto value = m_dem->getData(y, x);
    // Retorna o value do NODATA caso encontrá-lo
    if (value == m_dem->getNoDataValue())
	{
        return static_cast<short>(m_dem->getNoDataValue());
	}

    //Matriz D8 - 8 vizinhos:
    //0  1  2
    //3  x  4
    //5  6  7

    //Calcula o value da declividade para cada um dos vizinhos
    //Aquele que tiver a maior declividade é para onde irá a direção de fluxo
    //Para os sentidos 0, 2, 5 e 7 o comprimento é igual a Raiz(2)
    //Para os sentidos 1, 3, 4 e 6 o comprimento é igual a 1

    auto sqrt2 = static_cast<float>(std::sqrt(2));
    m_matrixD8[0] = (value - m_dem->getData(y - 1, x - 1)) / sqrt2;
    m_matrixD8[1] = value - m_dem->getData(y - 1, x);
    m_matrixD8[2] = (value - m_dem->getData(y - 1, x + 1)) / sqrt2;
    m_matrixD8[3] = value - m_dem->getData(y, x - 1);
    m_matrixD8[4] = value - m_dem->getData(y, x + 1);
    m_matrixD8[5] = (value - m_dem->getData(y + 1, x - 1)) / sqrt2;
    m_matrixD8[6] = value - m_dem->getData(y + 1, x);
    m_matrixD8[7] = (value - m_dem->getData(y + 1, x + 1)) / sqrt2;

    //32	64	    128
    //16	0	    1      'Configuração das direções de fluxo
    //8	4	    2

    float max = 0.01F;
    // value inicial que indica que não há direção de fluxo
    short enumMax = -1;

    // Seleciona a maior declividade dentre as calculadas
    for (short i = 0; i <= 7; ++i)
    {
        if (m_matrixD8[i] > max)
        {
            max = m_matrixD8[i];
            enumMax = i;
        }
    }

    // Identifica o sentido conforme o número do enumerador máximo
    switch (enumMax)
    {
        case 0:
            return 32;
        case 1:
            return 64;
        case 2:
            return 128;
        case 3:
            return 16;
        case 4:
            return 1;
        case 5:
            return 8;
        case 6:
            return 4;
        case 7:
            return 2;
        default:
            return 0;
    }
}

}
}
