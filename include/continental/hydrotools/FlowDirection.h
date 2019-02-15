#ifndef CONTINENTAL_HYDROTOOLS_FLOWDIRECTION_H
#define CONTINENTAL_HYDROTOOLS_FLOWDIRECTION_H

#include <QString>
#include <vector>
#include <cmath>
#include <stdexcept>
#include <memory>
#include <continental/datamanagement/Raster.h>

//*******************************************************************
//DIREÇÕES DE FLUXO (com base na codificação IDRISI)
//Criado por Vinícius Alencar Siqueira - 20/01/2014
//*******************************************************************

namespace continental
{
namespace hydrotools
{
/// <summary>
/// Calcula as direções de fluxo a partir de um MDE sem depressões
/// </summary>
/// <remarks></remarks>
class FlowDirection
{
    private:
        // Rasters
        std::unique_ptr<datamanagement::Raster<short>> m_MDE;
        std::unique_ptr<datamanagement::Raster<short>> m_flowDirection;
        // Matriz dos 8 vizinhos
        float m_matrixD8[8];
        size_t m_countDepressions = 0;

    public:
        /// <summary>
        /// Retorna o MDE original ou modificado pelo processo
        /// </summary>
        datamanagement::Raster<short> getMDEMatrix();

        /// <summary>
        /// Retorna o Flow Direction
        /// </summary>
        datamanagement::Raster<short> getFlowDirection();

        FlowDirection();

        //Lê um arquivo de MDE, especificando o local
        void readMDEdata(const QString &file);

        //Identifica as direções de fluxo e as depressões espúrias do MDE
        void calculateFlowDirection();

    private:
        //Verifica se algum vizinho de mesma cota, ou inferior já possui flow direction, atribuindo a mesma em caso verdadeiro
        void verifyFlowDirAtBounds(size_t xc, size_t yc, bool &validated);

        //Calcula a Direção de fluxo nos limites da grade
        void flowDirectionAtBounds();

        //Retorna a direção de fluxo de uma célula qualquer, posicionada em x,y; Função das declividades das células vizinhas
        short incipientFlowDirection(size_t x, size_t y);
};
}
}

#endif // CONTINENTAL_HYDROTOOLS_FLOWDIRECTION_H
