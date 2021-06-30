set(MODULE_NAME chroma )

# DEFINE THE TARGET
set( LIBNAME RAT_${MODULE_NAME} )

set( HEADERS ChromaInterface.hh )
if ( RAT_HAS_ZMQ )
  set( HEADERS ${HEADERS} zhelpers.hpp zmq.hpp )
endif()

set( SOURCES ChromaInterface.cc )
if ( RAT_HAS_ZMQ )
  set( SOURCES ${SOURCES} zhelpers.cc )
endif()
if ( RAT_HAS_PROTOBUF )
  set( SOURCES ${SOURCES} photonHit.pb.cc ratchromadata.pb.cc )
endif()
add_library( ${LIBNAME} SHARED ${SOURCES} )

# create a property to hold our headers
set_target_properties(${LIBNAME} PROPERTIES PUBLIC_HEADER "${HEADERS}")

# DEBUG MSG
#message( STATUS "chroma debug: ${Protobuf_LIBRARIES}")
#message( STATUS "chroma debug: ${Protobuf_INCLUDE_DIRS}")

get_target_property(ALL_HEADERS ${LIBNAME} PUBLIC_HEADER)

target_include_directories(${LIBNAME}
  PUBLIC
  $<INSTALL_INTERFACE:include/RAT/${MODULE_NAME}>
  ${RAT_INCLUDE_DIR}
  PRIVATE
  )

if ( RAT_HAS_ZMQ )
  set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_HAS_ZMQ")
endif()

if (RAT_HAS_PROTOBUF)
  set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_HAS_PROTOBUF")
endif()

if (HAS_CHROMA_INTERFACE AND RAT_HAS_PROTOBUF AND RAT_HAS_ZMQ )
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_HAS_CHROMA_INTERFACE")
endif()

target_link_libraries(${LIBNAME} RAT_DS ${Protobuf_LIBRARIES} ${ZeroMQ_LIBRARY} )

include_directories( ${CMAKE_CURRENT_SOURCE_DIR} ${PROJECT_SOURCE_DIR} ${Geant4_INCLUDE_DIRS} ${Protobuf_INCLUDE_DIRS} )
#ROOT_GENERATE_DICTIONARY(G__${LIBNAME} ${ALL_HEADERS} ${HEADERS} LINKDEF LinkDef.hh) # no dictionary needed
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
