cmake_minimum_required(VERSION 3.5)

include(FindPackageHandleStandardArgs)

find_path(SHP_INCLUDE_DIR shapefil.h PATHS "C:/git/shapelib/install/debug/include")

find_library(SHP_LIBRARY NAMES shp PATHS C:/git/shapelib/install/debug/lib)

#-----------------------------------------------------------------------
# handle the QUIETLY and REQUIRED arguments and set SQLite3_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(SHP
  FOUND_VAR
    SHP_FOUND
  REQUIRED_VARS
    SHP_INCLUDE_DIR
    SHP_LIBRARY
  VERSION_VAR
    SHP_VERSION
    )

mark_as_advanced(SHP_INCLUDE_DIR SHP_LIBRARY)

if(SHP_FOUND)
  set(SHP_INCLUDE_DIRS ${SHP_INCLUDE_DIR})
  set(SHP_LIBRARIES ${SHP_LIBRARY})

  if(NOT TARGET SHP::SHP)
    add_library(SHP::SHP UNKNOWN IMPORTED)
    set_target_properties(SHP::SHP PROPERTIES
      INTERFACE_INCLUDE_DIRECTORIES     "${SHP_INCLUDE_DIRS}"
      IMPORTED_LOCATION                 "${SHP_LIBRARY}"
      IMPORTED_LINK_INTERFACE_LANGUAGES "C"
      )
  endif()
endif()