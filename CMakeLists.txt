cmake_minimum_required(VERSION 3.5)
project(ContinentalHydroTools VERSION 1.0.0 LANGUAGES CXX)

add_definitions(-DMAKE_CONTINENTALDATAMANAGEMENT_LIB)
add_definitions(-DMAKE_CONTINENTALHYDROTOOLS_LIB)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed
set(CMAKE_AUTOMOC ON)
# Create code from a list of Qt designer ui files
set(CMAKE_AUTOUIC ON)

# Load custom configuration file for CMake (if any).
if(EXISTS ${CMAKE_SOURCE_DIR}/CMakeLists.config)
    include(${CMAKE_SOURCE_DIR}/CMakeLists.config)
endif()

#Make sure that custom modules like FindRapidJSON are found
list(INSERT CMAKE_MODULE_PATH 0 ${CMAKE_SOURCE_DIR}/cmake)

##############################################
# Declare dependencies
find_package(SHP REQUIRED)
include_directories(${SHP_INCLUDE_DIR})

find_package(ContinentalDataManagement REQUIRED)
include_directories(${ContinentalDataManagement_INCLUDE_DIRS})

find_package(Qt5Core CONFIG REQUIRED)

find_package(OpenMP)

# find_package(HDF5 REQUIRED COMPONENTS C CXX)
include_directories(${HDF5_INCLUDE_DIRS})
##############################################
# Create target and set properties

add_library(ContinentalHydroTools
    src/Version.cpp
    src/Readme.cpp
    src/constant/MensageConstant.cpp
    src/util/HDF5Util.cpp
    src/util/HydroToolsUtil.cpp
    src/service/Catchment.cpp
    src/service/Cell.cpp
    src/service/CellWatershed.cpp
    src/service/FlowAccumulation.cpp
    src/service/FlowDirection.cpp
    src/service/HeuristicCell.cpp
    src/service/HeuristicSinkRemoval.cpp
    src/service/HeuristicSinkRemovalUtil.cpp
    src/service/JunctionCell.cpp
#    src/LengthSegmentation.cpp
    src/service/shape/Box.cpp
    src/service/shape/Part.cpp
    src/service/shape/Point.cpp
    src/service/shape/ShapeFile.cpp
    src/service/shape/ShapeObject.cpp
    src/service/StreamAccumLength.cpp
    src/service/StreamDefinition.cpp
    src/service/StreamSegmentation.cpp
    src/domain/HydrotoolsProcessType.cpp
    src/domain/FileCommand.cpp
    src/domain/AbstractCommandInput.cpp
    src/domain/WatershedDelineationCommandInput.cpp
    src/domain/StreamSegmentationCommandInput.cpp
    src/domain/StreamDefinitionCommandInput.cpp
    src/domain/HeuristicSinkRemovalCommandInput.cpp
    src/domain/CatchmentCommandInput.cpp
    src/domain/FlowAccumulationCommandInput.cpp
    src/domain/StreamDefinitionThresholdType.cpp
    src/command/AbstractCommand.cpp
    src/command/CatchmentCommand.cpp
    src/command/WatershedDelineationCommand.cpp
    src/command/SinkDestroyCommand.cpp
    src/command/FlowAccumulationCommand.cpp
    src/command/StreamDefinitionCommand.cpp
    src/command/StreamSegmentationCommand.cpp
    src/infrastructure/AbstractRasterIO.cpp
    src/infrastructure/RasterIO.cpp
    src/exception/CatchmentDelineationIsNotValidInputCommandException.cpp
    src/exception/CatchmentDelineationProcessException.cpp
    src/exception/FlowAccumulationIsNotValidInputCommandException.cpp
    src/exception/FlowAccumulationProcessException.cpp
    src/exception/SinkDestroyProcessException.cpp
    src/exception/SinkDestroyIsNotValidInputCommandException.cpp
    src/exception/StreamDefinitionProcessException.cpp
    src/exception/StreamDefinitionIsNotValidInputCommandException.cpp
    src/exception/StreamSegmentationProcessException.cpp
    src/exception/StreamSegmentationIsNotValidInputCommandException.cpp
    src/exception/WatershedDelineationIsNotValidInputCommandException.cpp
    src/exception/WatershedDelineationProcessException.cpp
)

#Add an alias so that library can be used inside the build tree, e.g. when testing
add_library(ContinentalHydroTools::ContinentalHydroTools ALIAS ContinentalHydroTools)

#Set target properties
target_include_directories(ContinentalHydroTools
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_features(ContinentalHydroTools PRIVATE cxx_auto_type)
target_compile_options(ContinentalHydroTools PRIVATE $<$<CXX_COMPILER_ID:GNU>:-Wall>)

target_link_libraries(ContinentalHydroTools
    PUBLIC ContinentalDataManagement::ContinentalDataManagement
    PUBLIC SHP::SHP
    PUBLIC Qt5::Core
    PUBLIC OpenMP::OpenMP_CXX
    PUBLIC ${HDF5_LIBRARIES}
)

##############################################
# Installation instructions

include(GNUInstallDirs)
set(INSTALL_CONFIGDIR ${CMAKE_INSTALL_LIBDIR}/cmake/ContinentalHydroTools)

install(TARGETS ContinentalHydroTools
    EXPORT ContinentalHydroTools-targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

#This is required so that the exported target has the name ContinentalHydroTools and not ContinentalHydroTools
set_target_properties(ContinentalHydroTools PROPERTIES EXPORT_NAME ContinentalHydroTools)

install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

#Export the targets to a script
install(EXPORT ContinentalHydroTools-targets
  FILE
    ContinentalHydroToolsTargets.cmake
  NAMESPACE
    ContinentalHydroTools::
  DESTINATION
    ${INSTALL_CONFIGDIR}
)

#Create a ConfigVersion.cmake file
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/ContinentalHydroToolsConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

configure_package_config_file(${CMAKE_CURRENT_LIST_DIR}/cmake/ContinentalHydroToolsConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/ContinentalHydroToolsConfig.cmake
    INSTALL_DESTINATION ${INSTALL_CONFIGDIR}
)

#Install the config, configversion and custom find modules
install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/ContinentalHydroToolsConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/ContinentalHydroToolsConfigVersion.cmake
    DESTINATION ${INSTALL_CONFIGDIR}
)

export(EXPORT ContinentalHydroTools-targets FILE ${CMAKE_CURRENT_BINARY_DIR}/ContinentalHydroToolsTargets.cmake NAMESPACE ContinentalHydroTools::)

add_subdirectory(test)
add_subdirectory(console)
