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

# DEFINE SOME VARS THAT MIGHT BE USEFUL FOR SUBMODULES
message("python include: ${PYTHON_INCLUDE_PATH}")

get_target_property(ALL_HEADERS ${LIBNAME} PUBLIC_HEADER)

target_include_directories(${LIBNAME}
  PUBLIC
  $<INSTALL_INTERFACE:include/RAT/${MODULE_NAME}>
  ${RAT_INCLUDE_DIR}
  PRIVATE
  )
target_link_libraries(${LIBNAME} RAT_chroma)

include_directories( ${CMAKE_CURRENT_SOURCE_DIR} ${PROJECT_SOURCE_DIR} ${Geant4_INCLUDE_DIRS} )
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
