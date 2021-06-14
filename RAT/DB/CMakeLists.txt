set(MODULE_NAME DB )

# DEFINE THE TARGET
set( LIBNAME RAT_${MODULE_NAME} )

set( HEADERS DBExceptions.hh
DBFieldCallback.hh
DB.hh
DBJsonLoader.hh
DBLink.hh
DBTable.hh
DBTextLoader.hh
HTTPDownloader.hh
json.hh )

add_library( ${LIBNAME} SHARED DB.cc
DBJsonLoader.cc
DBLink.cc
DBTable.cc
DBTextLoader.cc
HTTPDownloader.cc
json.cc )

# create a property to hold our headers
set_target_properties(${LIBNAME} PROPERTIES PUBLIC_HEADER "${HEADERS}")

# DEFINE SOME VARS THAT MIGHT BE USEFUL FOR SUBMODULES
message("python include: ${PYTHON_INCLUDE_PATH} ${ROOT_LIBRARIES}")

get_target_property(ALL_HEADERS ${LIBNAME} PUBLIC_HEADER)

target_include_directories(${LIBNAME}
  PUBLIC
  $<INSTALL_INTERFACE:include/RAT/${MODULE_NAME}>
  ${RAT_INCLUDE_DIR}
  PRIVATE
  )
target_link_libraries(${LIBNAME} ${ROOT_LIBRARIES} ${ROOT_LIBRARY_DIR}/libPyROOT.so )

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
