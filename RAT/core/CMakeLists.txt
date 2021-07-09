set(MODULE_NAME core )

# DEFINE THE TARGET
set( LIBNAME RAT_${MODULE_NAME} )

set( HEADERS CountProc.hh
  GLG4HitPhoton.hh
  GLG4HitPMTCollection.hh
  GLG4HitPMT.hh
  GLG4SteppingAction.hh
  GLG4VEventAction.hh
  GLG4VisManager.hh
  Gsim.hh
  ProcAllocator.hh
  PruneProc.hh
  PythonProc.hh
  RunManager.hh
  Trajectory.hh )

add_library( ${LIBNAME} SHARED ConstructUserProc.cc
CountProc.cc
GLG4HitPhoton.cc
GLG4HitPMT.cc
GLG4HitPMTCollection.cc
GLG4SteppingAction.cc
GLG4VEventAction.cc
GLG4VisManager.cc
Gsim.cc
PruneProc.cc
PythonProc.cc
RunManager.cc
Trajectory.cc )

# create a property to hold our headers
set_target_properties(${LIBNAME} PROPERTIES PUBLIC_HEADER "${HEADERS}")

# DEBUG MSG
#message("core msg: G4VIS_USE=${Geant4_LIBRARIES}")

get_target_property(ALL_HEADERS ${LIBNAME} PUBLIC_HEADER)

target_include_directories(${LIBNAME}
  PUBLIC
  $<INSTALL_INTERFACE:include/RAT/${MODULE_NAME}>
  ${RAT_INCLUDE_DIR}
  PRIVATE
  )
if (HAS_CHROMA_INTERFACE)
  set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DHAS_CHROMA_INTERFACE=1")
else()
  set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DHAS_CHROMA_INTERFACE=0")  
endif()

if (RAT_USE_OPENGL)
  target_link_libraries(${LIBNAME} RAT_chroma ${Geant4_LIBRARIES} Geant4::G4OpenGL)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DRAT_USE_OPENGL")
else()
  target_link_libraries(${LIBNAME} RAT_chroma ${Geant4_LIBRARIES})
endif()

set( MODULE_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR} ${PROJECT_SOURCE_DIR} ${Geant4_INCLUDE_DIRS} )
if (Marley_FOUND)
  set( MODULE_INCLUDE_DIRS ${MODULE_INCLUDE_DIRS} ${MARLEY_INCLUDE_DIR} )
  set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DRAT_HAS_MARLEY")  
endif()

include_directories( ${MODULE_INCLUDE_DIRS} )
#ROOT_GENERATE_DICTIONARY(G__${LIBNAME} ${ALL_HEADERS} ${HEADERS} LINKDEF LinkDef.h) # no dictionary needed
#target_sources( ${LIBNAME} PRIVATE G__${LIBNAME}.cxx )

# install libraries and headers
install(TARGETS ${LIBNAME}
  EXPORT RAT
  LIBRARY DESTINATION lib
  PUBLIC_HEADER DESTINATION include/RAT/${MODULE_NAME})

# install files needed for dictionary use in CINT/CLING/PYTHON
#install( FILES
#  ${CMAKE_CURRENT_BINARY_DIR}/lib${LIBNAME}_rdict.pcm
#  ${CMAKE_CURRENT_BINARY_DIR}/lib${LIBNAME}.rootmap
#  DESTINATION lib  )
