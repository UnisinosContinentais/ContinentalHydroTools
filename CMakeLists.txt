cmake_minimum_required(VERSION 3.5)
project(ContinentalHydroTools VERSION 1.0.0 LANGUAGES CXX)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed
set(CMAKE_AUTOMOC ON)
# Create code from a list of Qt designer ui files
set(CMAKE_AUTOUIC ON)

#Make sure that custom modules like FindRapidJSON are found
list(INSERT CMAKE_MODULE_PATH 0 ${CMAKE_SOURCE_DIR}/cmake)

##############################################
# Declare dependencies
find_package(SHAPELIB REQUIRED)
include_directories(${SHAPELIB_INCLUDE_DIR})

find_package(ContinentalDataManagement REQUIRED)
include_directories(${ContinentalDataManagement_INCLUDE_DIRS})

find_package(Qt5Core CONFIG REQUIRED)

##############################################
# Create target and set properties

add_library(ContinentalHydroTools
    src/Catchment.cpp
    src/Cell.cpp
    src/CellWatershed.cpp
    src/FlowAccumulation.cpp
    src/FlowDirection.cpp
    src/HeuristicCell.cpp
    src/HeuristicSinkRemoval.cpp
    src/HeuristicSinkRemovalUtil.cpp
    src/JunctionCell.cpp
    src/LengthSegmentation.cpp
    src/shape/Box.cpp
    src/shape/Part.cpp
    src/shape/Point.cpp
    src/shape/ShapeFile.cpp
    src/shape/ShapeObject.cpp
    src/StreamAccumLength.cpp
    src/StreamDefinition.cpp
    src/StreamSegmentation.cpp
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
    PUBLIC Qt5::Core
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
#     ${CMAKE_CURRENT_LIST_DIR}/cmake/FindRapidJSON.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/ContinentalHydroToolsConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/ContinentalHydroToolsConfigVersion.cmake
    DESTINATION ${INSTALL_CONFIGDIR}
)

export(EXPORT ContinentalHydroTools-targets FILE ${CMAKE_CURRENT_BINARY_DIR}/ContinentalHydroToolsTargets.cmake NAMESPACE ContinentalHydroTools::)

#Register package in user's package registry
export(PACKAGE ContinentalHydroTools)

# add_subdirectory(test)
add_subdirectory(console)
