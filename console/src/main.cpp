/*
* Developed by UNISINOS
* author: Cristian Gabriel de Abreu Heylmann
* email: cristianheylmann@unisinos.br
* date: October, 2019
*/

#ifndef CONTINENTAL_HYDROTOOLS_TEST_H
#define  CONTINENTAL_HYDROTOOLS_TEST_H
#include <iostream>
#include <continental/datamanagement/RasterFile.h>
#include <continental/hydrotools/Version.h>
#include <continental/hydrotools/service/HeuristicSinkRemoval.h>
#include <continental/hydrotools/service/FlowAccumulation.h>
#include <continental/hydrotools/service/StreamDefinition.h>
#include <continental/hydrotools/service/StreamSegmentation.h>
#include <continental/hydrotools/service/Catchment.h>
#include <continental/hydrotools/command/WatershedDelineationCommand.h>
#include <continental/hydrotools/command/CatchmentCommand.h>
#include <continental/hydrotools/command/SinkDestroyCommand.h>
#include <continental/hydrotools/command/FlowAccumulation.h>
#include <continental/hydrotools/command/StreamDefinitionCommand.h>
#include <continental/hydrotools/command/StreamSegmentationCommand.h>
#include <continental/hydrotools/domain/HeuristicSinkRemovalCommandInput.h>
#include <continental/hydrotools/domain/StreamDefinitionCommandInput.h>
#include <continental/hydrotools/domain/StreamSegmentationCommandInput.h>
#include <continental/hydrotools/domain/FlowAccumulationCommandInput.h>
#include <continental/hydrotools/domain/WatershedDelineationCommandInput.h>
#include <continental/hydrotools/domain/HydrotoolsProcessType.h>
#include <continental/hydrotools/command/AbstractCommand.h>
#include <continental/hydrotools/exception/CatchmentDelineationIsNotValidInputCommandException.h>
#include <continental/hydrotools/exception/CatchmentDelineationIsNotValidInputCommandException.h>
#include <memory>
#include <QString>
#include <QCoreApplication>

using namespace continental::hydrotools::service;
using namespace continental::hydrotools::command;
using namespace continental::hydrotools::domain;
using namespace continental::hydrotools::exception;
using namespace continental::datamanagement;
using namespace std;

void sinkDestroy(QStringList args)
{
    auto inputSinkDestroy = HeuristicSinkRemovalCommandInput(args);
    inputSinkDestroy.prepare();
    auto command = new continental::hydrotools::command::SinkDestroyCommand(inputSinkDestroy);
    command->execute();
}

void flowAccumulation(QStringList args)
{
    auto inputFlowAccumulation = FlowAccumulationCommandInput(args);
    inputFlowAccumulation.prepare();
    auto command = new continental::hydrotools::command::FlowAccumulationCommand(inputFlowAccumulation);
    command->execute();
}

void streamDefinition(QStringList args)
{
    auto inputStreamDefinition = StreamDefinitionCommandInput(args);
    inputStreamDefinition.prepare();
    auto command = new continental::hydrotools::command::StreamDefinitionCommand(inputStreamDefinition);
    command->execute();
}

void streamSegmention(QStringList args){
    auto inputStreamSegmention = StreamSegmentationCommandInput(args);
    inputStreamSegmention.prepare();
    auto command = new continental::hydrotools::command::StreamSegmentationCommand(inputStreamSegmention);
    command->execute();
}

void catchmentDelineation(QStringList args)
{
    auto inputCatchment = CatchmentCommandInput(args);
    inputCatchment.prepare();
    auto command = new continental::hydrotools::command::CatchmentCommand(inputCatchment);
    command->execute();
}

void watershedDelineation(QStringList args)
{
    auto inputCatchment = WatershedDelineationCommandInput(args);
    inputCatchment.prepare();
    auto command = new continental::hydrotools::command::WatershedDelineationCommand(inputCatchment);
    command->execute();
}

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);
    QStringList args = app.arguments();
    try
    {
        if(args.length() > 1)
        {
            auto paramProcessModeInput = args[1].toInt();
            switch(paramProcessModeInput)
            {
                case HydrotoolsProcessType::ProgramTestMode:
                    std::cout << VER_FILE_VERSION_STR << endl;
                break;
                case HydrotoolsProcessType::SinkDestroy:
                    sinkDestroy(args);
                    break;
                case HydrotoolsProcessType::FlowAccumulation:
                    flowAccumulation(args);
                    break;
                case HydrotoolsProcessType::StreamDefinition:
                    streamDefinition(args);
                    break;
                case HydrotoolsProcessType::StreamSegmention:
                    streamSegmention(args);
                    break;
                case HydrotoolsProcessType::CatchmentDelineation:
                    catchmentDelineation(args);
                    break;
                case HydrotoolsProcessType::WatershedDelineation:
                    watershedDelineation(args);
                    break;
                default:
                    throw std::exception("Argumento do processo inválido. Padrão esperado [0 - 6]");
            }
        }
        else
        {
           throw std::exception("Argumento do processo inválido");
        }
     }
     catch (exception& e)
     {
       std::cout << "Erro ao executar comando no ContinentalHydroTools, erro: " << e.what() << endl;
       return 1;
     }
    return 0;
}

#endif
