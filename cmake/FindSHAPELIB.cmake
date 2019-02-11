cmake_minimum_required(VERSION 3.5)

include(FindPackageHandleStandardArgs)

find_path(SHAPELIB_INCLUDE_DIR shapelib/shapefil.h PATHS "C:/genesis/externals/win64-msvc2015/shapelib-1.4.1/include")
message("Pruuuu")
message(${SHAPELIB_INCLUDE_DIR})
find_library(SHAPELIB_LIBRARY NAMES shapelib PATHS C:/genesis/externals/win64-msvc2015/shapelib-1.4.1/lib)

#-----------------------------------------------------------------------
# handle the QUIETLY and REQUIRED arguments and set SQLite3_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(SHAPELIB
  FOUND_VAR
    SHAPELIB_FOUND
  REQUIRED_VARS
    SHAPELIB_INCLUDE_DIR
    SHAPELIB_LIBRARY
  VERSION_VAR
    SHAPELIB_VERSION
    )

mark_as_advanced(SHAPELIB_INCLUDE_DIR SHAPELIB_LIBRARY)

#-----------------------------------------------------------------------
# Set user variables and create imported targets if SQLite3 is found
if(SHAPELIB_FOUND)
  set(SHAPELIB_INCLUDE_DIRS ${SHAPELIB_INCLUDE_DIR})
  set(SHAPELIB_LIBRARIES ${SHAPELIB_LIBRARY})

  if(NOT TARGET SHAPELIB::SHAPELIB)
    add_library(SHAPELIB::SHAPELIB UNKNOWN IMPORTED)
    set_target_properties(SHAPELIB::SHAPELIB PROPERTIES
      INTERFACE_INCLUDE_DIRECTORIES     "${SHAPELIB_INCLUDE_DIRS}"
      IMPORTED_LOCATION                 "${SHAPELIB_LIBRARY}"
      IMPORTED_LINK_INTERFACE_LANGUAGES "C"
      )
  endif()
endif()