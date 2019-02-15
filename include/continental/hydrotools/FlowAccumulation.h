#ifndef CONTINENTAL_HYDROTOOLS_FLOWACCUMULATION_H
#define CONTINENTAL_HYDROTOOLS_FLOWACCUMULATION_H

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
/// <summary>
/// Classe que calcula o número de células acumuladas a partir de um raster com direções de fluxo
/// </summary>
class FlowAccumulation
{
private:
    std::shared_ptr<datamanagement::Raster<short>> m_flowDirection;
    std::vector<bool> m_checkedNodeList;
protected:
    std::shared_ptr<datamanagement::Raster<float>> m_flowAccumulation;
public:
    /// <summary>
    /// Retorna o MDE original ou modificado pelo processo
    /// </summary>
    std::shared_ptr<datamanagement::Raster<float>> getFlowAccumulation() const;

    /// <summary>
    /// Retorna o MDE original ou modificado pelo processo
    /// </summary>
    std::shared_ptr<datamanagement::Raster<short>> getFlowDirection() const;
    void setFlowDirection(std::shared_ptr<datamanagement::Raster<short>> flowDirection);

    //Cria um novo arquivo para
    FlowAccumulation();

    //Lê um arquivo de MDE, especificando o local
    void readFlowDirection(const QString &file);

    /// <summary>
    /// 'Calcula o número de células acumuladas de acordo com o Flow Direction selecionado
    /// </summary>
    virtual void runoff();

    //Escreve os dados de Flow Direction
    void writeFlowAccData(const QString &filepath);
private:
    //Verifica se algum vizinho de mesma cota, ou inferior já possui flow direction, atribuindo a mesma em caso verdadeiro
    bool neighbourCellsAnalyzed(int xc, int yc);
};
}
}

#endif // CONTINENTAL_HYDROTOOLS_FLOWACCUMULATION_H
