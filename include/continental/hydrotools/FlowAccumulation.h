#ifndef IPHYDRORASTERTOOLS_FLOWACCUMULATION_H
#define IPHYDRORASTERTOOLS_FLOWACCUMULATION_H

#include <QString>
#include <vector>
#include <memory>
#include <continental/datamanagement/Raster.h>

//*******************************************************************
//DETERMINAÇÃO DE ÁREA ACUMULADA
//Criado por Vinícius Alencar Siqueira - 20/01/2014
//*******************************************************************

namespace continental
{
namespace hydrotools
{
using namespace std;
using namespace continental::datamanagement;

/// <summary>
/// Classe que calcula o número de células acumuladas a partir de um raster com direções de fluxo
/// </summary>
class FlowAccumulation
{

private:
    shared_ptr<Raster<short>> m_flowDirection;
    Raster<float> m_flowAccumulation;
    std::vector<bool> m_checkedNodeList;

public:
    /// <summary>
    /// Retorna o MDE original ou modificado pelo processo
    /// </summary>
    Raster<float> getFlowAccumulationMatrix();

    /// <summary>
    /// Retorna o MDE original ou modificado pelo processo
    /// </summary>
    Raster<short> getFlowDirectionMatrix();
    void setFlowDirection(shared_ptr<Raster<short>> flowDirection);

    //Cria um novo arquivo para
    FlowAccumulation();

    //Lê um arquivo de MDE, especificando o local
    void readFlowDirection(const QString &file);

    /// <summary>
    /// 'Calcula o número de células acumuladas de acordo com o Flow Direction selecionado
    /// </summary>
    void runoff();

private:
    //Verifica se algum vizinho de mesma cota, ou inferior já possui flow direction, atribuindo a mesma em caso verdadeiro
    bool neighbourCellsAnalyzed(int xc, int yc);

public:
    //Escreve os dados de Flow Direction
    void writeFlowAccData(const QString &filepath);
};
}
}

#endif // IPHYDRORASTERTOOLS_FLOWACCUMULATION_H
