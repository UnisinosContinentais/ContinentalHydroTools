﻿/*
* Developed by UNISINOS
* author: Luiz Felipe Bertoldi de Oliveira
* email: lbertoldio@unisinos.br
* date: January, 2019
*/

#ifndef CONTINENTAL_HYDROTOOLS_STREAMDEFINITION_H
#define CONTINENTAL_HYDROTOOLS_STREAMDEFINITION_H

#include <QString>
#include <memory>
#include <continental/datamanagement/Raster.h>

//*******************************************************************
//IDENTIFICAÇÃO DE TRECHOS DE RIO
//*******************************************************************

namespace continental
{
namespace hydrotools
{
namespace service
{
/// <summary>
/// Classe utilizada para definir trechos de rio a partir de área acumulada
/// </summary>
class StreamDefinition
{

private:
    //Matriz de Fluxo acumulado
    std::shared_ptr<datamanagement::Raster<float>> m_flowAcc;
    //Matriz com os Grupos de treshold
    std::shared_ptr<datamanagement::Raster<float>> m_streamGroups;
    //Número mínimo de células acumuladas para formar uma rede de drenagem
    size_t m_threshold = 0;

protected:
    //Matriz com o Stream Definition
    std::shared_ptr<datamanagement::Raster<short>> m_streamDef;

public:
    /// <summary>
    /// Identifica o tipo de entrada das informações
    /// </summary>
    enum class ThresholdType
    {
        NumberOfCells = 0,
        PercentualOfMaximumCells = 1,
        Area = 2 //km²
    };

public:
    /// <summary>
    /// Retorna o valor do Threshold a ser utilizado
    /// </summary>
    size_t getThresholdValue() const;

    /// <summary>
    /// Retorna o o Stream Definition
    /// </summary>
    std::shared_ptr<datamanagement::Raster<short>> getStreamDefinition() const;

    void setStreamGroups(std::shared_ptr<datamanagement::Raster<float>> streamGroups) const;

    /// <summary>
    /// Retorna o o Stream Groups
    /// </summary>
    std::shared_ptr<datamanagement::Raster<float>> getStreamGroups() const;

    void setFlowAccumulation(std::shared_ptr<datamanagement::Raster<float>> flowAccumulation, float thresoldValue, ThresholdType thresoldType);

    /// <summary>
    /// Retorna a matriz de Flow Acumulation
    /// </summary>
    std::shared_ptr<datamanagement::Raster<float>> getFlowAccumulation() const;

    /// <summary>
    /// Cria um novo arquivo de StreamDefinition
    /// </summary>
    StreamDefinition();

    virtual ~StreamDefinition() = default;

    /// <summary>
    /// Rotina que define os trechos de rio.
    /// </summary>
    /// <remarks></remarks>
    virtual void defineStreams();
private:
    //Calcula o valor do threshold a ser adotado
    void calculateThreshold(float value, ThresholdType thrType);

    //Atribui um único grupo com treshold defiido
    void setUniqueTreshold();
};

}
}
}

#endif // CONTINENTAL_HYDROTOOLS_STREAMDEFINITION_H
