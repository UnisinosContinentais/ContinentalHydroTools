/*
* Developed by UNISINOS
* author: Luiz Felipe Bertoldi de Oliveira
* email: lbertoldio@unisinos.br
* date: January, 2019
*/

#ifndef CONTINENTAL_HYDROTOOLS_HEURISTICSINKREMOVAL_H
#define CONTINENTAL_HYDROTOOLS_HEURISTICSINKREMOVAL_H

#include <memory>
#include <QString>
#include <vector>
#include <cmath>
#include <stdexcept>
#include <continental/datamanagement/Raster.h>
#include "continental/hydrotools/HeuristicCell.h"
#include "continental/hydrotools/HeuristicSinkRemovalUtil.h"
#include <sstream>

//*******************************************************************
//REMOÇÃO DE DEPRESSÕES
//CÓDIGO MHS (Modified Heuristic Search) - v1.4
//Baseado no Artigo de Hou et al (2011) - Automatic Extraction of Drainage Networks from DEMs Base on Heuristic Search. Journal of Software, Vol. 6, nº 8
//*******************************************************************

namespace continental
{
namespace hydrotools
{
/// <summary>
/// Classe utilizada ara remover depressões - MHS, HS e PFS
/// </summary>
class HeuristicSinkRemoval
{
public:
    //Enumeradores para modo de processamento
    enum class ProcessingMode
    {
        // Utiliza o modo Modified Heuristic Search
        MHS = 0,
        // Utiliza o modo Heuristic Search
        HS = 1,
        // Utiliza o modo PFS
        PFS = 2
    };
protected:
    std::shared_ptr<datamanagement::Raster<short>> m_dem;
    std::shared_ptr<datamanagement::Raster<short>> m_flowDirection;
private:
    // Matriz dos 8 vizinhos
    float m_matrixD8[8];
    // Matriz de células sem Direcao de Fluxo - células com depressão
    Cell* m_array;
    size_t m_arraySize = 0;
    // Tamanho da janela é a dimensão para N x N do método orinal de Hou et al (2011);
    int m_windowSize = 5;
    size_t m_maxOpenList = 0;
    size_t m_maxClosedList = 0;
    // Peso da função de custo
    float m_weightFunctionCost = 0;

    //Matriz de direções para atribuir a direção de fluxo em área plana: esquerda, baixo, direita, cima, diagonais
    // Matriz de direções em X
    short m_directionsX[8] = {-1, 0, 1, 0, -1, -1, 1, 1};
    //Matriz de direções em Y
    short m_directionsY[8] = {0, -1, 0, 1, 1, -1, -1, 1};

    // Conjunto das células candidatas
    HeuristicCell* m_openList;
    // Conjunto das células selecionadas
    HeuristicCell* m_closedList;
    bool* m_closedListBoolean;
    bool* m_openListBoolean;
    size_t* m_closedListPosition;
    size_t* m_openListPosition;
    size_t* m_traceBackMatrix;
    // Método de remoção de depressões
    ProcessingMode m_algorithmMode = ProcessingMode::MHS;
	
	// Percentual de conclusão
	float percent = 0;
	bool error = true;
public:
    /// <summary>
    /// Cria uma nova instância da classe Removedora de depressões
    /// </summary>
    /// <param name="maxOpenList">Descrição: Número máximo de elementos na lista de células candidatas.</param>
    /// <param name="maxClosedList">Descrição: Número máximo de elementos na lista de células selecionadas.</param>
    /// <param name="weightFunctionCost">Descrição: Peso para a função de custo G(i)</param>
    /// <param name="processingAlgorithm">Descrição: Modo de processamento para a remoção de depressões.</param>
    /// <remarks></remarks>
    HeuristicSinkRemoval(size_t maxOpenList, size_t maxClosedList, float weightFunctionCost, HeuristicSinkRemoval::ProcessingMode processingAlgorithm);

    virtual ~HeuristicSinkRemoval();

    /// <summary>
    /// Retorna ou atribui o tamanho da janela que se move em torno da célula com depressão (somente modo Heuristic Search)
    /// </summary>
    int getWindowSize() const;
    void setWindowSize(int value);

    /// <summary>
    /// Retorna ou atribui o modo de remoção de depressões
    /// </summary>
    ProcessingMode getProcessMode() const;
    void setProcessMode(ProcessingMode value);

    /// <summary>
    /// Retorna o MDE original ou modificado pelo processo
    /// </summary>
    virtual std::shared_ptr<datamanagement::Raster<short>> getDem() const;

    void setDem(std::shared_ptr<datamanagement::Raster<short>> mde);

    /// <summary>
    /// Retorna o MDE original ou modificado pelo processo
    /// </summary>
    virtual std::shared_ptr<datamanagement::Raster<short>> getFlowDirection() const;

    /// <summary>
    /// Rotina para removção das depressões do MDE.
    /// </summary>
    virtual void removeSinks();
private:
    //Remove as depressões utilizando uma matriz 'n x n' para busca em torno da depressão espúria
    void removeDepressions();

    //Adiciona a célula com a depressão na openlist para ser analisada
    inline void addDepressionToOpenList(size_t xc, size_t yc, size_t &numberElements)
    {
        // Coloca a célula na open list
        m_openList[numberElements].valid = true;
        m_openList[numberElements].x = xc;
        m_openList[numberElements].y = yc;
        //Aciona o Flag para indicar que a célula está na open list
        m_openListBoolean[yc * m_dem->getCols() + xc] = true;
        m_openList[numberElements].relParentX = 0;
        m_openList[numberElements].relParentY = 0;
        // Identifica a posição da célula na matriz (na forma de vetor)
        m_openListPosition[numberElements] = yc * m_dem->getCols() + xc;

        numberElements += 1;
    }

    //Adiciona os elementos vizinhos na Open List, retornando o número de elementos adicionados
    inline void addNeighboursToOpenList(size_t yc, size_t xc, size_t &numberElementos)
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

                // Estabelece pointers relativos para o o nó de origem
                m_openList[numberElementos].valid = true;
                m_openList[numberElementos].x = static_cast<size_t>(posX);
                m_openList[numberElementos].y = static_cast<size_t>(posY);
                m_openList[numberElementos].relParentX = static_cast<int>(xc) - posX;
                m_openList[numberElementos].relParentY = static_cast<int>(yc) - posY;
                // Aciona o flag indicando que a célula está na open list
                m_openListBoolean[position] = true;
                // Indica a posição do ponto da open list
                m_openListPosition[numberElementos] = static_cast<size_t>(posY) * m_dem->getCols() + static_cast<size_t>(posX);
                numberElementos += 1;
            }
        }
    }


    //Realiza um ajuste final no MDE utilizando o flow direction acertado
    void adjustDemFinalElevations();

    //Faz um procedimento de "cavar" o MDE, utilizando o caminho encontrado
    inline void breaching(size_t closedListCount)
    {
        //identifica as elevações inicial e final da pathlist
        short initElevation = m_dem->getData(m_closedList[0].y, m_closedList[0].x);
        short finalElevation = m_dem->getData(m_closedList[closedListCount - 1].y, m_closedList[closedListCount - 1].x);

        //Evita que o programa utilize o value do NoDATA como elevação final, o que pode comprometer o resultado
        if (finalElevation == m_dem->getNoDataValue())
        {
            finalElevation = initElevation;
        }

        if (finalElevation > initElevation)
        {
            // Pode ocorrer em situações de limite do MDE, nesse caso, força o decréscimo na cota
            finalElevation = initElevation;
            m_dem->setData(m_closedList[closedListCount - 1].y, m_closedList[closedListCount - 1].x, finalElevation);
        }

        size_t numberCells = 0, xParent = 0, yParent = 0;
        bool foundStartNode = false;
        size_t enumerator = closedListCount - 1;

        // Identifica o nº de células no caminho do outlet até a depressão
        while (!foundStartNode)
        {
            // Acha a coordenada do parent node
            xParent = m_closedList[enumerator].x + static_cast<size_t>(m_closedList[enumerator].relParentX);
            yParent = m_closedList[enumerator].y + static_cast<size_t>(m_closedList[enumerator].relParentY);

            if (m_closedList[enumerator].relParentX == 0 && m_closedList[enumerator].relParentY == 0)
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
            xParent = m_closedList[enumerator].x + static_cast<size_t>(m_closedList[enumerator].relParentX);
            yParent = m_closedList[enumerator].y + static_cast<size_t>(m_closedList[enumerator].relParentY);
            //Atribuo o novo flow direction
            m_flowDirection->setData(yParent, xParent, HeuristicSinkRemovalUtil::relativeIncipientFlowDirection(static_cast<int>(xParent), static_cast<int>(m_closedList[enumerator].x), static_cast<int>(yParent), static_cast<int>(m_closedList[enumerator].y)));

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

    //Retorna o enumerador da célula com o maior valor heurístico
    inline size_t enumMinHeuristicInfo(size_t numberOpenlist)
    {
        float value = 99999;
        size_t enumerator = 0;

        for (int i = 0; i < numberOpenlist; ++i)
        {
            if (m_openList[i].valid && m_openList[i].costFunction < value)
            {
                value = m_openList[i].costFunction;
                enumerator = i;
            }
        }

        return enumerator;

    }

    //Preenche a depressão até a cota do vizinho mais próximo, de acordo com um treshold pré definido
    void fillDepressions(int differenceHeight);

    //Força a saída pelas bordas da parede
    inline void forceBoundOutlet(size_t yc, size_t xc, size_t maxClosedListElements, size_t &numberActualClosedListElements)
    {
        int elements = -1;

        // Verifica, dentre todas as células da closed list, a primeira célula que for de borda
        for (size_t w = 0; w < maxClosedListElements; ++w)
        {
            if (m_closedList[w].x == 0 || m_closedList[w].y == 0 || m_closedList[w].x == (m_dem->getCols() - 1) || m_closedList[w].y == (m_dem->getRows() - 1))
            {
                elements = static_cast<int>(w) + 1;
                break;
            }
        }

        // Caso não houver nenhuma célula de borda, dispara o erro
        if (elements == -1)
        {
            std::stringstream buffer("The maximum number of elements from the closed list was not enough to find the river section at X,Y = (");
            buffer << xc << "," << yc << ").";
            throw std::runtime_error(buffer.str());
        }

        numberActualClosedListElements = static_cast<size_t>(elements);
    }

    //Retorna o valor da função heurística para uma célula localizada na posição Xc, Yc
    inline float heuristicValue(int yc, int xc, short es)
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

    //Verifica se chegou em algum ponto de exutório
    inline bool isOutlet(short ei, short es, size_t posY, size_t posX)
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

    //Retorna o valor da função heurística modificada, para uma célula cujo nó pai (parent node) está localizado na posição Xc, Yc
    //Deve ser informada a posição relativa xRel, yRel em relação ao nó pai.
    inline float modifiedHeuristicValue(int yc, int xc, short es, short relPosY, short relPosX, float wg)
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

    //Retorna o valor da função custo PFS, para uma célula localizada na posição Yc, Xc
    inline float pfsValue(size_t yc, size_t xc, short es)
    {
        // ei = value da elevação na célula de indice 'i'
        // es = value da elevação na célula inicial (starting node)

        short ei = m_dem->getData(yc, xc);
        // value da função PFS(i) -> diferença de cota entre o nó inicial e o de índice 'i'
        return ei - es;
    }

    //Adiciona os elementos da open list na closed list, excluindo o valor central (que deve ir para o Path list)
    inline void pushCellToClosedList(size_t enumCell, size_t &numberClosed)
    {
        size_t position = m_openList[enumCell].y * m_dem->getCols() + m_openList[enumCell].x;

        // Aciona o Flag para indicar que a célula está na closed list
        m_closedListBoolean[position] = true;
        // Coloca a célula na closed list
        m_closedList[numberClosed].valid = true;
        m_closedList[numberClosed].x = m_openList[enumCell].x;
        m_closedList[numberClosed].y = m_openList[enumCell].y;
        m_closedList[numberClosed].relParentX = m_openList[enumCell].relParentX;
        m_closedList[numberClosed].relParentY = m_openList[enumCell].relParentY;

        //Identifica a posição da célula na matriz (na forma de vetor)
        m_closedListPosition[numberClosed] = position;
        //guarda a posição da célula para conhecer o caminho de volta
        m_traceBackMatrix[position] = numberClosed;

        //Retira o flag para indicar que a célula não está mais na open list
        m_openListBoolean[position] = false;
        // Retira a célula da open list
        m_openList[enumCell].valid = false;

        numberClosed += 1;
    }

    //Reinicia as open e closed lists, para a próxima depressão
    void resetAllList(size_t nClosedList, size_t nOpenList);

    //Identifica as direções de fluxo e as depressões espúrias do MDE
    void calculateFlowDirection();

    //Verifica se algum vizinho de mesma cota, ou inferior já possui flow direction, atribuindo a mesma em caso verdadeiro
    inline void verifyFlowDirAtBounds(size_t xc, size_t yc, bool &validated)
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

    //Calcula a Direção de fluxo nos limites da grade
    void flowDirectionAtBounds();

    //Retorna a direção de fluxo de uma célula qualquer, posicionada em x,y; Função das declividades das células vizinhas
    inline short incipientFlowDirection(size_t x, size_t y)
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
};

}
}

#endif // CONTINENTAL_HYDROTOOLS_HEURISTICSINKREMOVAL_H
