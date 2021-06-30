set(MODULE_NAME gen )

# DEFINE THE TARGET
set( LIBNAME RAT_${MODULE_NAME}  )

set( HEADERS BetaFunction.hh
CfGen.hh
CfSource.hh
Coincidence_Gen.hh
DecayChain_Gen.hh
DecayChain.hh
ESgen.hh
FermiFunction.hh
Gen_LED.hh
Gen_RandomTrigger.hh
GLG4DeferTrackProc.hh
GLG4Gen.hh
GLG4GenList.hh
GLG4PosGen.hh
GLG4PrimaryGeneratorAction.hh
GLG4PrimaryGeneratorMessenger.hh
GLG4TimeGen.hh
GLG4VertexGen.hh
IBDgen.hh
PosGen_FillShell.hh
PosGen_Line.hh
PosGen_Multipoint.hh
PosGen_Radial.hh
PosGen_RegexFill.hh
PosGen_TriMeshSurface.hh
PrimaryVertexInformation.hh
ReacIBDgen.hh
VertexFile_Gen.hh
VertexGen_ES.hh
VertexGen_IBD.hh
VertexGen_PhotonBomb.hh
VertexGen_ReacIBD.hh
VertexGen_Spectrum.hh
VertexGen_WIMP.hh )

set( MODULE_SOURCES BetaFunction.cc
CfGen.cc
CfSource.cc
Coincidence_Gen.cc
DecayChain.cc
DecayChain_Gen.cc
ESgen.cc
FermiFunction.cc
Gen_LED.cc
Gen_RandomTrigger.cc
GLG4DeferTrackProc.cc
GLG4Gen.cc
GLG4PosGen.cc
GLG4PrimaryGeneratorAction.cc
GLG4TimeGen.cc
GLG4VertexGen.cc
IBDgen.cc
PosGen_FillShell.cc
PosGen_Line.cc
PosGen_Multipoint.cc
PosGen_Radial.cc
PosGen_RegexFill.cc
PosGen_TriMeshSurface.cc
ReacIBDgen.cc
VertexFile_Gen.cc
VertexGen_ES.cc
VertexGen_IBD.cc
VertexGen_PhotonBomb.cc
VertexGen_ReacIBD.cc
VertexGen_Spectrum.cc
VertexGen_WIMP.cc )

if (Marley_FOUND)
  set( MODULE_SOURCES ${MODULE_SOURCES} VertexGen_Marley.cc )
  set( HEADERS ${HEADERS} VertexGen_Marley.hh )
endif()

add_library( ${LIBNAME} SHARED ${MODULE_SOURCES} )

# create a property to hold our headers
set_target_properties(${LIBNAME} PROPERTIES PUBLIC_HEADER "${HEADERS}")

get_target_property(ALL_HEADERS ${LIBNAME} PUBLIC_HEADER)

target_include_directories(${LIBNAME}
  PUBLIC
  $<INSTALL_INTERFACE:include/RAT/${MODULE_NAME}>
  ${RAT_INCLUDE_DIR}
  PRIVATE
  )


set( MODULE_LINK_LIBS ${LIBNAME} )
if (Marley_FOUND)
  set( MODULE_LINK_LIBS ${MODULE_LINK_LIBS} marley Marley_fftpack ${ROOT_LIBRARIES} ${GSL_LIBRARIES})
endif()
target_link_libraries(${MODULE_LINK_LIBS})

# DEFINE SOME VARS THAT MIGHT BE USEFUL FOR SUBMODULES
#message("msg gen: ${MODULE_LINK_LIBS}")

set( MODULE_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR} ${PROJECT_SOURCE_DIR} ${Geant4_INCLUDE_DIRS} )
if (Marley_FOUND)
  set( MODULE_INCLUDE_DIRS ${MODULE_INCLUDE_DIRS} ${MARLEY_INCLUDE_DIR} )
endif()

include_directories( ${MODULE_INCLUDE_DIRS} )
  
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
