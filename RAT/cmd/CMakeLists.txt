set(MODULE_NAME cmd )

# DEFINE THE TARGET
set( LIBNAME RAT_${MODULE_NAME}  )

set( HEADERS ChromaInterfaceMessenger.hh
CoincidenceMessenger.hh
DBMessenger.hh
ESgenMessenger.hh
GLG4DebugMessenger.hh
GLG4VisMessenger.hh
PhysicsListMessenger.hh
ProcBlockManager.hh
ReacIBDgenMessenger.hh
TrackingMessenger.hh )

add_library( ${LIBNAME} SHARED ChromaInterfaceMessenger.cc
CoincidenceMessenger.cc
DBMessenger.cc
ESgenMessenger.cc
GLG4DebugMessenger.cc
GLG4PrimaryGeneratorMessenger.cc
GLG4VisMessenger.cc
PhysicsListMessenger.cc
ProcBlockManager.cc
ReacIBDgenMessenger.cc
TrackingMessenger.cc )

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
target_link_libraries(${LIBNAME})

include_directories( ${CMAKE_CURRENT_SOURCE_DIR} ${PROJECT_SOURCE_DIR} ${Geant4_INCLUDE_DIRS} )
#ROOT_GENERATE_DICTIONARY(G__${LIBNAME} ${ALL_HEADERS} ${HEADERS} LINKDEF LinkDef.hh) # no dictionary needed
#target_sources( ${LIBNAME} PRIVATE G__${LIBNAME}.cxx )

# install libraries and headers
install(TARGETS ${LIBNAME}
  EXPORT RAT
  LIBRARY DESTINATION lib
  PUBLIC_HEADER DESTINATION include/RAT/${MODULE_NAME} )

# install files needed for dictionary use in CINT/CLING/PYTHON
#install( FILES
#  ${CMAKE_CURRENT_BINARY_DIR}/lib${LIBNAME}_rdict.pcm
#  ${CMAKE_CURRENT_BINARY_DIR}/lib${LIBNAME}.rootmap
#  DESTINATION lib  )
