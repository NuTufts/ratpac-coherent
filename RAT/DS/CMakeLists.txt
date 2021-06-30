set(MODULE_NAME DS )

# DEFINE THE TARGET
set( LIBNAME RAT_${MODULE_NAME} )

set( HEADERS Calib.hh
Centroid.hh
EV.hh
MC.hh
MCParticle.hh
MCPhoton.hh
MCPMT.hh
MCSummary.hh
MCTrack.hh
MCTrackStep.hh
PathFit.hh
PMT.hh
PMTInfo.hh
PosFit.hh
Root.hh
Run.hh
RunStore.hh )

add_library( ${LIBNAME} SHARED MCPMT.cc
Root.cc
RunStore.cc )

# create a property to hold our headers
set_target_properties(${LIBNAME} PROPERTIES PUBLIC_HEADER "${HEADERS}")

# MSG FOR DEBUG
#message( STATUS "DS debug: " )

get_target_property(ALL_HEADERS ${LIBNAME} PUBLIC_HEADER)

target_include_directories(${LIBNAME}
  PUBLIC
  $<INSTALL_INTERFACE:include/RAT/${MODULE_NAME}>
  ${RAT_INCLUDE_DIR}
  PRIVATE
  )
target_link_libraries(${LIBNAME} ${ROOT_LIBRARIES} )

include_directories( ${CMAKE_CURRENT_SOURCE_DIR} ${PROJECT_SOURCE_DIR} )
ROOT_GENERATE_DICTIONARY(G__${LIBNAME} ${ALL_HEADERS} ${HEADERS} LINKDEF LinkDef.hh) # no dictionary needed
target_sources( ${LIBNAME} PRIVATE G__${LIBNAME}.cxx )

# install libraries and headers
install(TARGETS ${LIBNAME}
  EXPORT RAT
  LIBRARY DESTINATION lib
  PUBLIC_HEADER DESTINATION include/RAT/${MODULE_NAME})

# install files needed for dictionary use in CINT/CLING/PYTHON
install( FILES
  ${CMAKE_CURRENT_BINARY_DIR}/lib${LIBNAME}_rdict.pcm
  ${CMAKE_CURRENT_BINARY_DIR}/lib${LIBNAME}.rootmap
  DESTINATION lib  )
