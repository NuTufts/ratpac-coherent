set(MODULE_NAME geo )

# DEFINE THE TARGET
set( LIBNAME RAT_${MODULE_NAME}  )

set( HEADERS BWVetGenericChamber.hh
BWVetGenericChamberHit.hh
ConeWaveguideConstruction.hh
ConeWaveguideFactory.hh
DetectorConstruction.hh
DetectorFactory.hh
GeoBoxFactory.hh
GeoBubbleFactory.hh
GeoBuilder.hh
GeoConvexLensFactory.hh
GeoCutTubeFactory.hh
GeoFactory.hh
GeoLensFactory.hh
GeoPerfBoxFactory.hh
GeoPerfSphereFactory.hh
GeoPerfTubeFactory.hh
GeoPMTArrayFactory.hh
GeoPMTCoverageFactory.hh
GeoPMTFactoryBase.hh
GeoPMTParser.hh
GeoPolygonFactory.hh
GeoReflectorFactory.hh
GeoReflectorWaveguideFactory.hh
GeoRevArrayFactory.hh
GeoRevolutionChimneyFactory.hh
GeoRevolutionFactory.hh
GeoSkinSurfaceFactory.hh
GeoSolidArrayFactoryBase.hh
GeoSolidFactory.hh
GeoSphereFactory.hh
GeoSurfaceFactory.hh
GeoTorusFactory.hh
GeoTubeArrayFactory.hh
GeoTubeFactory.hh
GeoTubeIntersectionFactory.hh
GeoWatchmanShieldFactory.hh
GeoWaterBoxArrayFactory.hh
GLG4BoxSD.hh
GLG4PMTSD.hh
GLG4SimpleOpDetSD.hh
GLG4TestSolid.hh
GLG4TorusStack.hh
Materials.hh
PMTConcentrator.hh
PMTConstruction.hh
TheiaDetectorFactory.hh
TubeFacetSolid.hh
UnionSolidArray.hh
WatchmanDetectorFactory.hh
WaterBoxConstruction.hh
WaveguideFactory.hh )

add_library( ${LIBNAME} SHARED BWVetGenericChamber.cc
BWVetGenericChamberHit.cc
ConeWaveguideConstruction.cc
ConeWaveguideFactory.cc
DetectorConstruction.cc
DetectorFactory.cc
GeoBoxFactory.cc
GeoBubbleFactory.cc
GeoBuilder.cc
GeoConvexLensFactory.cc
GeoCutTubeFactory.cc
GeoFactory.cc
GeoLensFactory.cc
GeoPerfBoxFactory.cc
GeoPerfSphereFactory.cc
GeoPerfTubeFactory.cc
GeoPMTArrayFactory.cc
GeoPMTCoverageFactory.cc
GeoPMTFactoryBase.cc
GeoPMTParser.cc
GeoPolygonFactory.cc
GeoReflectorFactory.cc
GeoReflectorWaveguideFactory.cc
GeoRevArrayFactory.cc
GeoRevolutionChimneyFactory.cc
GeoRevolutionFactory.cc
GeoSkinSurfaceFactory.cc
GeoSolidArrayFactoryBase.cc
GeoSolidFactory.cc
GeoSphereFactory.cc
GeoSurfaceFactory.cc
GeoTorusFactory.cc
GeoTubeArrayFactory.cc
GeoTubeFactory.cc
GeoTubeIntersectionFactory.cc
GeoWatchmanShieldFactory.cc
GeoWaterBoxArrayFactory.cc
GLG4BoxSD.cc
GLG4PMTSD.cc
GLG4SimpleOpDetSD.cc
GLG4TestSolid.cc
GLG4TorusStack.cc
Materials.cc
PMTConcentrator.cc
PMTConstruction.cc
TheiaDetectorFactory.cc
TubeFacetSolid.cc
UnionSolidArray.cc
WatchmanDetectorFactory.cc
WaterBoxConstruction.cc )

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
