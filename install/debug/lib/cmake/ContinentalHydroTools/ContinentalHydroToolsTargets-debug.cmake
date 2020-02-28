#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "ContinentalHydroTools::ContinentalHydroTools" for configuration "Debug"
set_property(TARGET ContinentalHydroTools::ContinentalHydroTools APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(ContinentalHydroTools::ContinentalHydroTools PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "CXX"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/ContinentalHydroTools.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS ContinentalHydroTools::ContinentalHydroTools )
list(APPEND _IMPORT_CHECK_FILES_FOR_ContinentalHydroTools::ContinentalHydroTools "${_IMPORT_PREFIX}/lib/ContinentalHydroTools.lib" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
