get_filename_component(ContinentalHydroTools_CMAKE_DIR "${CMAKE_CURRENT_LIST_FILE}" PATH)
include(CMakeFindDependencyMacro)

list(APPEND CMAKE_MODULE_PATH ${ContinentalHydroTools_CMAKE_DIR})
list(REMOVE_AT CMAKE_MODULE_PATH -1)

if(NOT TARGET ContinentalHydroTools::ContinentalHydroTools)
    include("${ContinentalHydroTools_CMAKE_DIR}/ContinentalHydroToolsTargets.cmake")
endif()

set(ContinentalHydroTools_LIBRARIES ContinentalHydroTools::ContinentalHydroTools)
