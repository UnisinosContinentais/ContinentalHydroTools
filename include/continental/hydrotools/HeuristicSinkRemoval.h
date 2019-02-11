#ifndef IPHYDRORASTERTOOLS_HEURISTICSINKREMOVAL_H
#define IPHYDRORASTERTOOLS_HEURISTICSINKREMOVAL_H

#include <memory>
#include <QString>
#include <vector>
#include <cmath>
#include <stdexcept>
#include <continental/dataManagement/Raster.h>
#include "continental/hydrotools/HeuristicCell.h"

//*******************************************************************
//REMOÇÃO DE DEPRESSÕES
//Criado por Vinícius Alencar Siqueira - 20/01/2014
//CÓDIGO MHS (Modified Heuristic Search) - v1.4
//Baseado no Artigo de Hou et al (2011) - Automatic Extraction of Drainage Networks from DEMs Base on Heuristic Search. Journal of Software, Vol. 6, nº 8
//*******************************************************************

namespace continental
{
namespace hydrotools
{
using namespace std;
using namespace continental::dataManagement;

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
private:
    // Rasters
    Raster<short> m_MDE;
    Raster<short> m_flowDirection;
    // Matriz dos 8 vizinhos
    float m_matrixD8[8];
    // Matriz de células sem Direcao de Fluxo - células com depressão
    vector<unique_ptr<Cell>> m_array;
    // Tamanho da janela é a dimensão para N x N do método orinal de Hou et al (2011);
    int m_windowSize = 5;
    size_t m_maxOpenList = 0;
    size_t m_maxClosedList = 0;
    // Peso da função de custo
    float m_weightFuncG = 0;

    //Matriz de direções para atribuir a direção de fluxo em área plana: esquerda, baixo, direita, cima, diagonais
    // Matriz de direções em X
    short m_directionsX[8] = {-1, 0, 1, 0, -1, -1, 1, 1};
    //Matriz de direções em Y
    short m_directionsY[8] = {0, -1, 0, 1, 1, -1, -1, 1};

    // Conjunto das células candidatas
    vector<unique_ptr<HeuristicCell>> m_openList;
    // Conjunto das células selecionadas
    vector<unique_ptr<HeuristicCell>> m_closedList;
    vector<bool> m_closedListBoolean;
    vector<bool> m_openListBoolean;
	size_t m_ListBooleanRows = 0;
	size_t m_listBooleanCols = 0;
    vector<size_t> m_closedListPosition;
    vector<size_t> m_openListPosition;
    vector<size_t> m_traceBackMatrix;
	size_t m_traceBackMatrixRows = 0;
	size_t m_traceBackMatrixCols = 0;
    vector<bool> m_forcedOutlets;
	size_t m_forcedOutletsRows = 0;
	size_t m_forcedOutletsCols = 0;
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
	/// <param name="weightFunctionG">Descrição: Peso para a função de custo G(i)</param>
	/// <param name="processingAlgorithm">Descrição: Modo de processamento para a remoção de depressões.</param>
	/// <remarks></remarks>
	HeuristicSinkRemoval(size_t maxOpenList, size_t maxClosedList, float weightFunctionG, HeuristicSinkRemoval::ProcessingMode processingAlgorithm);

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
    Raster<short> getMDEMatrix();

    /// <summary>
    /// Retorna o MDE original ou modificado pelo processo
    /// </summary>
    Raster<short> getFlowDirectionMatrix();

    //Lê um arquivo de MDE, especificando o local
    void readMDEdata(const QString &file);

    /// <summary>
    /// Rotina para removção das depressões do MDE.
    /// </summary>
    void removeSinks();
private:
    //Remove as depressões utilizando uma matriz 'n x n' para busca em torno da depressão espúria
    void removeDepressions();

    //Adiciona a célula com a depressão na openlist para ser analisada
    void addDepressionToOpenList(size_t xc, size_t yc, size_t &numberElements);

    //Adiciona os elementos vizinhos na Open List, retornando o número de elementos adicionados
    void addNeighboursToOpenList(size_t yc, size_t xc, size_t &numberElementos);


    //Realiza um ajuste final no MDE utilizando o flow direction acertado
    void adjustDemFinalElevations();

    //Faz um procedimento de "cavar" o MDE, utilizando o caminho encontrado
    void breaching(size_t closedListCount);

    //Retorna o enumerador da célula com o maior valor heurístico
    size_t enumMinHeuristicInfo(size_t numberOpenlist);

    //Preenche a depressão até a cota do vizinho mais próximo, de acordo com um treshold pré definido
    void fillDepressions(int differenceHeight);

    //Força a saída pelas bordas da parede
    void forceBoundOutlet(size_t yc, size_t xc, size_t maxClosedListElements, size_t &numberActualClosedListElements);

    //Retorna o valor da função heurística para uma célula localizada na posição Xc, Yc
    float heuristicValue(int yc, int xc, short es);

    //Verifica se chegou em algum ponto de exutório
    bool isOutlet(short ei, short es, size_t posY, size_t posX);

    //Retorna o valor da função heurística modificada, para uma célula cujo nó pai (parent node) está localizado na posição Xc, Yc
    //Deve ser informada a posição relativa xRel, yRel em relação ao nó pai.
    float modifiedHeuristicValue(int yc, int xc, short es, short relPosY, short relPosX, float wg);

    //Retorna o valor da função custo PFS, para uma célula localizada na posição Yc, Xc
    float PFSValue(size_t yc, size_t xc, short es);

    //Adiciona os elementos da open list na closed list, excluindo o valor central (que deve ir para o Path list)
    void pushCellToClosedList(size_t enumCell, size_t &numberClosed);

    //Libera da memória todas as matrizes utilizadas nas operações
    void releaseMemory();

    //Reinicia as open e closed lists, para a próxima depressão
    void resetAllList(size_t nClosedList, size_t nOpenList);

    //Identifica as direções de fluxo e as depressões espúrias do MDE
    void calculateFlowDirection();

    //Verifica se algum vizinho de mesma cota, ou inferior já possui flow direction, atribuindo a mesma em caso verdadeiro
    void verifyFlowDirAtBounds(size_t xc, size_t yc, bool &validated);

    //Calcula a Direção de fluxo nos limites da grade
    void flowDirectionAtBounds();

    //Retorna a direção de fluxo de uma célula qualquer, posicionada em x,y; Função das declividades das células vizinhas
    short incipientFlowDirection(size_t x, size_t y);
};
}
}

#endif // IPHYDRORASTERTOOLS_HEURISTICSINKREMOVAL_H
