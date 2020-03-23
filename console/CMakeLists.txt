add_definitions(-DMAKE_CONTINENTALDATAMANAGEMENT_LIB)
add_definitions(-DMAKE_CONTINENTALHYDROTOOLS_LIB)

add_executable(ContinentalHydroToolsConsole src/main.cpp)
target_compile_features(ContinentalHydroToolsConsole PRIVATE cxx_auto_type)

target_link_libraries(ContinentalHydroToolsConsole ContinentalHydroTools::ContinentalHydroTools)

IF (WIN32)
    file(COPY ${SHP_DLL} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

    install(
        FILES		${CMAKE_CURRENT_BINARY_DIR}/ContinentalHydroToolsConsole.exe
        DESTINATION	console
    )

    install(
        FILES		${SHP_DLL}
        DESTINATION	console
    )
ENDIF()
