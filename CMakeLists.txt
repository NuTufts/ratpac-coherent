cmake_minimum_required(VERSION 3.10)
project(RAT VERSION 1.0.0)

#---Install location by default is in the build directory
if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set (CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/installed" CACHE PATH "${CMAKE_BUILD_DIR}" FORCE )
endif()

#---Module directory
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

## Dependencies

#--- CERN ROOT
find_package(ROOT CONFIG REQUIRED)

#--- CERN GEANT4
find_package(Geant4 CONFIG REQUIRED)

set( USE_PYTHON2 0)

if (USE_PYTHON3)
  find_package( PythonInterp 3 REQUIRED )
  find_package( PythonLibs 3 REQUIRED )
  # warning gross hack
  execute_process( COMMAND python3 -c "import numpy; print(numpy.get_include())" OUTPUT_VARIABLE NUMPY_INCLUDES OUTPUT_STRIP_TRAILING_WHITESPACE )
  #message(${NUMPY_INCLUDES})
  set(RAT_USE_PYTHON 1)
  set(RAT_PYTHON_VERSION 3)
elseif(USE_PYTHON2)
  find_package( PythonInterp 2.7 REQUIRED )
  find_package( PythonLibs 2.7 REQUIRED )
  find_package( Numpy REQUIRED )
  set(RAT_USE_PYTHON 1)
  set(RAT_PYTHON_VERSION 2)
else()
  set(RAT_USE_PYTHON 0)
    set(RAT_PYTHON_VERSION 0)
endif()

## Setup

#---Define useful ROOT functions and macros (e.g. ROOT_GENERATE_DICTIONARY)
include(${ROOT_USE_FILE})

#---Define Variables for subdirectory cmakelists
set(RAT_BASE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")

## Add Source

#---Add base of source
add_subdirectory(RAT)

#---Configuration export
include(CMakePackageConfigHelpers)
set(RAT_INCLUDE_DIR "${CMAKE_INSTALL_PREFIX}/include")
set(RAT_LIBRARY_DIR "${CMAKE_INSTALL_PREFIX}/lib")
set(TARGETS_EXPORT_NAME "RATTargets")
configure_package_config_file("cmake/Config.cmake.in"
  "${CMAKE_BINARY_DIR}/cmake/RATConfig.cmake"
  INSTALL_DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/RAT"
  PATH_VARS RAT_INCLUDE_DIR RAT_LIBRARY_DIR RAT_BASE_DIR RAT_USE_PYTHON RAT_PYTHON_VERSION)
write_basic_package_version_file("${CMAKE_BINARY_DIR}/cmake/RATConfigVersion.cmake"
  COMPATIBILITY SameMajorVersion)


## Export Targets

#---Create info to include LArCV in other cmake files
install(EXPORT RAT
        FILE RATTargets.cmake
        DESTINATION "${CMAKE_BINARY_DIR}/installed/lib/cmake/RAT")

#---Create info to include RAT in other cmake files
install(FILES "${CMAKE_BINARY_DIR}/cmake/RATConfig.cmake"
  "${CMAKE_BINARY_DIR}/cmake/RATConfigVersion.cmake"
  DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/RAT")
