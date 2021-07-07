cmake_minimum_required(VERSION 3.10)
project(RAT VERSION 1.0.0)

#---Install location by default is in the build directory
if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set (CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/installed" CACHE PATH "${CMAKE_BUILD_DIR}" FORCE )
endif()

#---Module directory
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

#--- TURN ON CHROMA OPTION: SET to 1 if want it ON, default is 0 ---
set(HAS_CHROMA_INTERFACE 0)

## Dependencies

#--- CERN ROOT
#find_package(XercesC CONFIG REQUIRED)

#--- CERN ROOT
find_package(ROOT CONFIG REQUIRED)

#--- CERN GEANT4
find_package(Geant4 CONFIG REQUIRED)

#--- MARLEY
find_package(Marley PATHS ${CMAKE_SOURCE_DIR}/extern/marley/build/cmake/Marley)
if ( Marley_FOUND )
  message( STATUS "Found Marley")  
  # --- need GSL
  find_package( GSL REQUIRED )
else()
    message( STATUS "Marley not built. To include it, go to extern/marley and build it.")  
endif()

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

if (RAT_USE_OPENGL)
  set(G4VIS_USE_OPENGLX 1)
  set(G4VIS_USE_OPENGLXM 1)
  set(G4VIS_USE_OPENGLQT 1)
  set(G4VIS_USE 1)
  message( STATUS "Using OpenGL" )
else()
  message( STATUS "Not using OpenGL" )
endif()

find_package( Protobuf )
if ( Protobuf_FOUND )
  message( STATUS "Protobuf version : ${Protobuf_VERSION}" )
  message( STATUS "Protobuf include path : ${Protobuf_INCLUDE_DIRS}" )
  message( STATUS "Protobuf libraries : ${Protobuf_LIBRARIES}" )
  set( RAT_HAS_PROTOBUF 1 )
else()
  message( WARNING "Protobuf package not found -> specify search path via PROTOBUF_ROOT variable")
  set( RAT_HAS_PROTOBUF 0 )  
endif()

## ------ ZEROMQ ------

if ( HAS_CHROMA_INTERFACE )
  #find_package( ZeroMQ REQUIRED )
  ## load in pkg-config support
  find_package(PkgConfig)
  ## use pkg-config to get hints for 0mq locations
  pkg_check_modules(PC_ZeroMQ QUIET zmq)
  
  ## use the hint from above to find where 'zmq.hpp' is located
  find_path(ZeroMQ_INCLUDE_DIR
    NAMES zmq.hpp
    PATHS ${PC_ZeroMQ_INCLUDE_DIRS}
    )

  ## use the hint from above to find the location of libzmq
  find_library(ZeroMQ_LIBRARY
    NAMES zmq
    PATHS ${PC_ZeroMQ_LIBRARY_DIRS}
    )
  message( STATUS "ZeroMQ Library: ${ZeroMQ_LIBRARY}" )
  set( RAT_HAS_ZMQ 1 )
else()
  set( RAT_HAS_ZMQ 0 )  
endif()
      
## Set C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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

#-- executable
add_subdirectory(RAT/bin)

## Export Targets

#---Create info to include LArCV in other cmake files
install(EXPORT RAT
        FILE RATTargets.cmake
        DESTINATION "${CMAKE_BINARY_DIR}/installed/lib/cmake/RAT")

#---Create info to include RAT in other cmake files
install(FILES "${CMAKE_BINARY_DIR}/cmake/RATConfig.cmake"
  "${CMAKE_BINARY_DIR}/cmake/RATConfigVersion.cmake"
  DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/RAT")
