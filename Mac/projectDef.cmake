#/**********************************************************\ 
# Auto-generated Mac project definition file for the
# linphone-js project
#\**********************************************************/

# Mac template platform definition CMake file
# Included from ../CMakeLists.txt

# remember that the current source dir is the project root; this file is in Mac/
file (GLOB PLATFORM RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    Mac/[^.]*.cpp
    Mac/[^.]*.h
    Mac/[^.]*.cmake
    )


# linphone libraries
SET(LIBS_PATH "/Users/system/Downloads/Linphone.app/Contents/Resources/lib/")
SET(PLUGIN_INTERNAL_DEPS ${PLUGIN_INTERNAL_DEPS}
	${LIBS_PATH}/liblinphone.4.dylib
	${LIBS_PATH}/libmediastreamer.1.dylib
	${LIBS_PATH}/libortp.8.dylib
	${LIBS_PATH}/libspeex.1.dylib
	${LIBS_PATH}/libosipparser2.4.dylib
	${LIBS_PATH}/libosip2.4.dylib
)

# use this to add preprocessor definitions
add_definitions(
)


SOURCE_GROUP(Mac FILES ${PLATFORM})

set (SOURCES
    ${SOURCES}
    ${PLATFORM}
    )

set(PLIST "Mac/bundle_template/Info.plist")
set(STRINGS "Mac/bundle_template/InfoPlist.strings")
set(LOCALIZED "Mac/bundle_template/Localized.r")

add_mac_plugin(${PROJECT_NAME} ${PLIST} ${STRINGS} ${LOCALIZED} SOURCES)

# add library dependencies here; leave ${PLUGIN_INTERNAL_DEPS} there unless you know what you're doing!
target_link_libraries(${PROJECT_NAME}
    ${PLUGIN_INTERNAL_DEPS}
    )
