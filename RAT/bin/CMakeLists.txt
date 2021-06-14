
set( EXENAME rat )

add_executable(rat
  rat.cc )

# create a property to hold our headers
target_include_directories(${EXENAME}
  PUBLIC
  ${RAT_INCLUDE_DIR}
  PRIVATE
  )
message("rat libs: ${RAT_LIBRARY_DIR} ${RAT_INCLUDE_DIR}")

set( RAT_LIBS RAT_stlplus RAT_DS RAT_base RAT_DB RAT_io RAT_daq RAT_gen RAT_io RAT_fit RAT_core RAT_physics RAT_geo RAT_cmd )
target_link_libraries(${EXENAME} ${RAT_LIBS} ${Geant4_LIBRARIES} ${ROOT_LIBRARIES} )

include_directories( ${CMAKE_CURRENT_SOURCE_DIR} ${PROJECT_SOURCE_DIR} ${Geant4_INCLUDE_DIRS})
#ROOT_GENERATE_DICTIONARY(G__${LIBNAME} ${ALL_HEADERS} ${HEADERS} LINKDEF LinkDef.hh) # no dictionary needed
#target_sources( ${LIBNAME} PRIVATE G__${LIBNAME}.cxx )

# install libraries and headers
install(TARGETS ${EXENAME}
  RUNTIME DESTINATION bin )
