#include "continental/hydrotools/StreamAccumLength.h"
#include "continental/hydrotools/FlowDirection.h"

using namespace continental::dataManagement;

namespace IPHydroRasterTools
{

	StreamAccumLength::StreamAccumLength()
	{
		m_flowDirection = new RasterInteger();
	}

	void StreamAccumLength::readFlowDirectionData(const std::QString &local)
	{
		m_flowDirection = new RasterInteger();
		m_flowDirection->ReadData(local);
	}

	void StreamAccumLength::readStreamDefinitionData(const std::QString &local)
	{
		m_strDef = new RasterInteger();
		m_strDef->ReadData(local);
		m_accumLength = new RasterReal(m_strDef->Linhas, m_strDef->Colunas, m_strDef->XllCorner, m_strDef->YllCorner, m_strDef->Cellsize, m_strDef->NoDataValue);
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

		for (auto row = 0; row < m_strDef->Linhas; row++)
		{
			for (auto col = 0; col < m_strDef->Colunas; col++)
			{
				i = row;
				j = col;

				if (m_strDef->Dados(i, j) == m_strDef->NoDataValue)
				{
					m_accumLength->Dados(i, j) = m_accumLength->NoDataValue; //Atribui valores de NODATA caso não for drenagem

				}
				else if (m_strDef->Dados(i, j) > 0) //Caso estiver sobre a drenagem
				{
					if (m_accumLength->Dados(i, j) == 0) //Caso a célula estiver sobre um trecho que não tenha sido calculado o comprimento
					{

						if (headwaterFound((short)i, (short)j, false) == true) //Se for cabeceira de drenagem
						{

							do
							{
		ReachVerification:
								xAnterior = j; //Identifica a linha e a coluna iniciais
								yAnterior = i;
								FlowDirection::moveToFlowDirection(m_flowDirection->Dados(i, j), i, j); //Move em direção à próxima célula

								if (i < 0 || j < 0 || i > m_strDef->Linhas - 1 || j > m_strDef->Colunas - 1) //Se alcançar os limites da matriz, Não calcula nada e cai fora do loop
								{
									Length = 0; //Reinicia o comprimento acumulado
									break;
								}

								if (m_flowDirection->Dados(i, j) == m_flowDirection->NoDataValue)
								{
									m_accumLength->Dados(i, j) = m_accumLength->NoDataValue;
									Length = 0; //Reinicia o comprimento acumulado
									break; //Passa se mover para um NODATA
								}

								JunctionFound = verifyStreamOutlet((short)i, (short)j, false); //Verifica se chegou em uma junção
								if (JunctionFound == true)
								{

									Length += continental::dataManagement::SpatialAnalyst::CellLength(yLat, Xlon, (i - yAnterior), (j - xAnterior), m_strDef->Cellsize); //Calcula o comprimento acumulado até a célula anterior à junção
									m_accumLength->Dados(yAnterior, xAnterior) = Length * 1000; //Atribui o comprimento acumulado para a célula anterior (em metros), com base nas direções relativas
									Length = 0; //Reinicia o comprimento acumulado para o novo trecho
									goto ReachVerification; //Começa a partir do novo trecho
								}

								//Calcula a longitude e a latitude da célula em questão
								yLat = (float)(m_flowDirection->YllCorner + (m_flowDirection->Linhas - 1 - i) * m_flowDirection->Cellsize + (m_flowDirection->Cellsize / 2));
								Xlon = (float)(m_flowDirection->XllCorner + (j * m_flowDirection->Cellsize) + (m_flowDirection->Cellsize / 2));

								Length += continental::dataManagement::SpatialAnalyst::CellLength(yLat, Xlon, (i - yAnterior), (j - xAnterior), m_strDef->Cellsize); //Calcula o comprimento acumulado até a célula atual, de acordo com
								m_accumLength->Dados(yAnterior, xAnterior) = Length * 1000; //Atribui o comprimento acumulado para a célula anterior (em metros), com base nas direções relativas
							} while (true);

							JunctionFound = false;
						}
					}
				}
			}
		}
	}

	void StreamAccumLength::segmentStreamsByLength(float MaxLength)
	{

		m_strDef = nullptr; // A matriz do stream definition é liberada para otimizar a memória
		GC::Collect();
		m_strSeg = new RasterInteger(m_flowDirection->Linhas, m_flowDirection->Colunas, m_flowDirection->XllCorner, m_flowDirection->YllCorner, m_flowDirection->Cellsize, m_flowDirection->NoDataValue);

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

		for (auto row = 0; row < m_accumLength->Linhas; row++)
		{
			for (auto col = 0; col < m_accumLength->Colunas; col++)
			{
				i = row;
				j = col;

				if (m_accumLength->Dados(i, j) == m_accumLength->NoDataValue)
				{
					m_strSeg->Dados(i, j) = m_accumLength->NoDataValue; //Atribui valores de NODATA caso não for drenagem

				}
				else if (m_accumLength->Dados(i, j) > 0) //Caso estiver sobre a drenagem
				{

					if (headwaterFound((short)i, (short)j, true) == true) //Se for cabeceira de drenagem
					{
						xInicial = j; //Identifica a linha e a coluna iniciais
						yInicial = i;

						do
						{
		ReachVerification:
							if (m_strSeg->Dados(i, j) > 0) //Caso a célula estiver sobre um trecho que já tenha sido segmentado, sai do Loop
							{
								break;
							}

							xFinal = j; //Identifica a linha e a coluna anteriores
							yFinal = i;
							FlowDirection::moveToFlowDirection(m_flowDirection->Dados(i, j), i, j); //Move em direção à próxima célula

							if (i < 0 || j < 0 || i > m_strSeg->Linhas - 1 || j > m_strSeg->Colunas - 1) //Se alcançar os limites da matriz, Não calcula nada e cai fora do loop
							{
								break;
							}

							if (m_flowDirection->Dados(i, j) == m_flowDirection->NoDataValue)
							{

								TotalLength = m_accumLength->Dados(yFinal, xFinal); //Verifica o comprimento acumulado do trecho
								updateReaches(yInicial, xInicial, i, j, (short)SegmentNumber, IncreasedSegments, (TotalLength / MaxLength), TotalLength);
								SegmentNumber += IncreasedSegments; //Aumenta o número dos segmentos identificados
								break; //Passa se mover para um NODATA

							}

							JunctionFound = verifyStreamOutlet((short)i, (short)j, true); //Verifica se chegou em uma junção
							if (JunctionFound == true)
							{

								TotalLength = m_accumLength->Dados(yFinal, xFinal); //Verifica o comprimento acumulado do trecho
								updateReaches(yInicial, xInicial, yFinal, xFinal, (short)SegmentNumber, IncreasedSegments, (TotalLength / MaxLength), TotalLength);

								SegmentNumber += IncreasedSegments; //Aumenta o número dos segmentos identificados

								xInicial = j; //A linha e a coluna iniciais passam a ser do novo trecho
								yInicial = i;
								goto ReachVerification; //Começa a partir do novo trecho

							}
						} while (true);

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
		int nReach_final = std::ceil(nReaches); //Retorna o número de trechos (arredonda para cima)
		float maxReachLength = ReachLength / nReach_final; //Identifica o novo limite de comprimento para o trecho

		m_strSeg->Dados(LinhaFinal, ColunaFinal) = Atributo + (nReach_final - 1); //Insere o valor do segmentation final para o "exutório" do trecho

		while (!(lin == LinhaFinal && col == ColunaFinal)) //Faça até chegar na célula final do trecho
		{

			m_strSeg->Dados(lin, col) = Atributo + (varAtributo - 1);
			if (m_accumLength->Dados(lin, col) > (varAtributo * maxReachLength)) //Se o comprimento atual for maior do que o permitido para o trecho, aumenta o número do segmento
			{
				varAtributo += 1;
			}

			FlowDirection::moveToFlowDirection(m_flowDirection->Dados(lin, col), lin, col);
			if (col < 0 || lin < 0 || col > (m_flowDirection->Colunas - 1) || lin > (m_flowDirection->Linhas - 1)) //Cheguei na borda do MDE
			{
				break;
			}
		}

		SegmentNumberIncrease = varAtributo; //Retorna o número de trechos adicionados

	}

	bool StreamAccumLength::headwaterFound(short Yc, short xc, bool SearchAccumLength)
	{
		//direções de apontamento para a célula central
		//For y = -1 To 1       2  4  8       
		//posY = (Yc + y)       1    x   16       
		//For x = -1 To 1       128   64  32       
		//posX = (xc + x)

		short xi = 0;
		short yi = 0;
		short nPointingCells = 0;

		for (auto y = -1; y <= 1; y++)
		{
			yi = (Yc + y);
			for (auto x = -1; x <= 1; x++)
			{
				xi = (xc + x);

				if (x != 0 || y != 0) //Evita a análise da própria célula (nó central)
				{
					if (xi >= 0 && yi >= 0 && xi < m_flowDirection->Colunas && yi < m_flowDirection->Linhas) //Evita sair dos limites do raster
					{


						if (SearchAccumLength == false)
						{
							if (m_strDef->Dados(yi, xi) > 0) //Somente se a célula faz parte da rede de drenagem principal
							{
                                if (FlowDirection::relativeIncipientFlowDirection(xi, xc, yi, Yc) == m_flowDirection->Dados(yi, xi)) //Se a célula analisada apontar para a célula de origem, é contabilizada
								{
									nPointingCells += 1;
								}
							}
						}
						else
						{
							if (m_accumLength->Dados(yi, xi) > 0) //Somente se a célula faz parte da rede de drenagem principal
							{
                                if (FlowDirection::relativeIncipientFlowDirection(xi, xc, yi, Yc) == m_flowDirection->Dados(yi, xi)) //Se a célula analisada apontar para a célula de origem, é contabilizada
								{
									nPointingCells += 1;
								}
							}
						}

					}
				}
			}
		}

		if (nPointingCells == 0) //Se nenhuma célula apontar para a célula central, significa que encontrou uma cabeceira
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	bool StreamAccumLength::verifyStreamOutlet(short Yc, short xc, bool SearchAccumLength)
	{

		//direções de apontamento para a célula central
		//For y = -1 To 1       2  4  8       
		//posY = (Yc + y)       1    x   16       
		//For x = -1 To 1       128   64  32       
		//posX = (xc + x)

		short xi = 0;
		short yi = 0;
		short nPointingCells = 0;

		for (auto y = -1; y <= 1; y++)
		{
			yi = (Yc + y);
			for (auto x = -1; x <= 1; x++)
			{
				xi = (xc + x);

				if (x != 0 || y != 0) //Evita a análise da própria célula (nó central)
				{
					if (xi >= 0 && yi >= 0 && xi < m_flowDirection->Colunas && yi < m_flowDirection->Linhas) //Evita sair dos limites do raster
					{

						if (SearchAccumLength == false)
						{
							if (m_strDef->Dados(yi, xi) > 0) //Somente se a célula faz parte da rede de drenagem principal
							{
                                if (FlowDirection::relativeIncipientFlowDirection(xi, xc, yi, Yc) == m_flowDirection->Dados(yi, xi)) //Se a célula analisada apontar para a célula de origem, é contabilizada
								{
									nPointingCells += 1;
								}
							}
						}
						else
						{
							if (m_accumLength->Dados(yi, xi) > 0) //Somente se a célula faz parte da rede de drenagem principal
							{
                                if (FlowDirection::relativeIncipientFlowDirection(xi, xc, yi, Yc) == m_flowDirection->Dados(yi, xi)) //Se a célula analisada apontar para a célula de origem, é contabilizada
								{
									nPointingCells += 1;
								}
							}
						}

					}
				}
			}
		}

		if (nPointingCells > 1) //Se a mais de uma célula apontar para a célula central, significa que encontrou uma confluência
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	void StreamAccumLength::writeAccumLengthData(const std::QString &arquivo)
	{
		m_accumLength->WriteData(arquivo);
	}

	void StreamAccumLength::WriteSegmentedStreams(const std::QString &arquivo)
	{
		m_strSeg->WriteData(arquivo);
	}
}
