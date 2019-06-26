#include "continental/hydrotools/Cell.h"
#include "continental/hydrotools/HeuristicSinkRemoval.h"

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

}
}
