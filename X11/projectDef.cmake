#/**********************************************************\ 
# Auto-generated X11 project definition file for the
# linphone-js project
#\**********************************************************/

# X11 template platform definition CMake file
# Included from ../CMakeLists.txt

# remember that the current source dir is the project root; this file is in X11/
file (GLOB PLATFORM RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    X11/[^.]*.cpp
    X11/[^.]*.h
    X11/[^.]*.cmake
    )

SOURCE_GROUP(X11 FILES ${PLATFORM})

# linphone libraries
SET(PLUGIN_INTERNAL_DEPS ${PLUGIN_INTERNAL_DEPS}
	${CORE_PATH}/coreapi/.libs/liblinphone.so
	${CORE_PATH}/mediastreamer2/src/.libs/libmediastreamer.so
	${CORE_PATH}/oRTP/src/.libs/libortp.so
	-lspeex
	-losipparser2
	-losip2
)


# use this to add preprocessor definitions
add_definitions(
)

set (SOURCES
    ${SOURCES}
    ${PLATFORM}
    )

add_x11_plugin(${PROJECT_NAME} SOURCES)

# add library dependencies here; leave ${PLUGIN_INTERNAL_DEPS} there unless you know what you're doing!
target_link_libraries(${PROJECT_NAME}
    ${PLUGIN_INTERNAL_DEPS}
    )
