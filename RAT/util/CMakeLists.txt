set(MODULE_NAME util )

# DEFINE THE TARGET
set( LIBNAME RAT_${MODULE_NAME}  )

set( HEADERS CentroidCalculator.hh
cuda_error.hh
Factory.hh
GaussianRatioPDF.hh
GLG4StringUtil.hh
HashFunc.hh
LinearInterp.hh
ListHelp.hh
MultiChargeDist.hh
PolygonOrientation.hh
RadicalInverse.hh
ReadFile.hh
Sampling.hh
SimpsIntegral.hh
SimulatedAnnealing.hh
StringUtil.hh
TimeSort.hh
TimeUtil.hh )

add_library( ${LIBNAME} SHARED GaussianRatioPDF.cc
getopt1.cc
getopt.cc
GLG4StringUtil.cc
MultiChargeDist.cc
PolygonOrientation.cc
ReadFile.cc
Sampling.cc
SilenceRooFitBanner.cc
SimpsIntegral.cc
StringUtil.cc )


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
