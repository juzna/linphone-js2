#/**********************************************************\ 
# 
# Auto-generated CMakeLists.txt for the linphone-js project
#
#\**********************************************************/

# Written to work with cmake 2.6
cmake_minimum_required (VERSION 2.6)
set (CMAKE_BACKWARDS_COMPATIBILITY 2.6)

Project(${PLUGIN_NAME})

file (GLOB GENERAL RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    [^.]*.cpp
    [^.]*.h
    [^.]*.cmake
    )

INCLUDE_DIRECTORIES(${PLUGIN_INCLUDE_DIRS})


# add linphone core includes
SET(LINPHONE_CORE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/linphone/")
INCLUDE_DIRECTORIES(
  ${LINPHONE_CORE_PATH}/oRTP/include
  ${LINPHONE_CORE_PATH}/mediastreamer2/include
  ${LINPHONE_CORE_PATH}/coreapi
  ${LINPHONE_CORE_PATH}/
)

# Generated files are stored in ${GENERATED} by the project configuration
SET_SOURCE_FILES_PROPERTIES(
    ${GENERATED}
    PROPERTIES
        GENERATED 1
    )

SOURCE_GROUP(Generated FILES
    ${GENERATED}
    )

SET( SOURCES
    ${GENERAL}
    ${GENERATED}
    )

#SET(CPACK_GENERATOR "STGZ;TGZ;TZ;ZIP")
#SET(CPACK_GENERATOR "ZIP")
#SET(CPACK_PACKAGE_NAME "linphone-js")
#SET(CPACK_PACKAGE_FILE_NAME "linphone-js")

# This will include Win/projectDef.cmake, X11/projectDef.cmake, Mac/projectDef 
# depending on the platform
include_platform()

#message("Will generate ${CPACK_GENERATOR}")

INSTALL(TARGETS ${PLUGIN_NAME}
  DESTINATION lib/mozilla/plugins
#  RUNTIME DESTINATION bin
#  LIBRARY DESTINATION lib
#  ARCHIVE DESTINATION lib
)

# must be last one, because it overwrites variables otherwise
INCLUDE(CPack)
