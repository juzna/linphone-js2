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

# linphone core libraries
#add_subdirectory(${LINPHONE_CORE_PATH})

add_library(LIB_CORE SHARED IMPORTED)
set_property(TARGET LIB_CORE PROPERTY IMPORTED_LOCATION ${LINPHONE_CORE_PATH}/coreapi/.libs/liblinphone.so)

SET(LINPHONE_LIBS
	${LINPHONE_CORE_PATH}/coreapi/.libs/liblinphone.so
    #LIB_CORE
	${LINPHONE_CORE_PATH}/mediastreamer2/src/.libs/libmediastreamer.so
	${LINPHONE_CORE_PATH}/oRTP/src/.libs/libortp.so
)

find_library(LIB_SPEEX speex)
find_library(LIB_SIPPARSER osipparser2)
find_library(LIB_OSIP osip2)


#INSTALL(TARGETS LIB_CORE
#  RUNTIME DESTINATION bin
#  LIBRARY DESTINATION lib
#  ARCHIVE DESTINATION lib
#)


message("Found libs in ${LIB_SPEEX}, ${LIB_SIPPARSER}, ${LIB_OSIP}")

SET(MORE_LIBS
	${LIB_SPEEX}
	${LIB_SIPPARSER}
	${LIB_OSIP}
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
    ${LINPHONE_LIBS}
    ${MORE_LIBS}
    )

SET(CPACK_GENERATOR "${CPACK_GENERATOR};DEB")
SET(CPACK_PACKAGE_VENDOR "juzna.cz")
SET(CPACK_DEBIAN_PACKAGE_NAME linphone-js)
SET(CPACK_DEBIAN_PACKAGE_VERSION 0.1)
SET(CPACK_DEBIAN_PACKAGE_ARCHITECTURE i386)
SET(CPACK_DEBIAN_PACKAGE_MAINTAINER "Jan Dolecek <juzna.cz@gmail.com>")
SET(CPACK_DEBIAN_PACKAGE_DESCRIPTION "Linphone wrapper for JavaScript")

SET(DEPS_MEDIASTREAMER "libasound2 (>> 1.0.22), libavcodec52 (>= 4:0.5+svn20090706-3) | libavcodec-extra-52 (>= 4:0.5+svn20090706-3), libgsm1 (>= 1.0.13), libogg0 (>= 1.0rc3), libsdl1.2debian (>= 1.2.10-1), libspeex1 (>= 1.2~beta3-1), libspeexdsp1 (>= 1.2~beta3.2-1), libswscale0 (>= 4:0.5+svn20090706-3) | libswscale-extra-0 (>= 4:0.5+svn20090706-3), libtheora0 (>= 0.0.0.alpha7.dfsg), libx11-6")
SET(DEPS_CORE "libspeex1, libosip2-4, libexosip2-4, libortp-dev, libmediastreamer-dev")
SET(CPACK_DEBIAN_PACKAGE_DEPENDS "${DEPS_MEDIASTREAMER},${DEPS_CORE}")


# file(INSTALL ${LINPHONE_LIBS} DESTINATION lib)

SET(LINPHONE_LIBS_R
	${LINPHONE_CORE_PATH}/coreapi/.libs/liblinphone.so
	${LINPHONE_CORE_PATH}/coreapi/.libs/liblinphone.so.4
	${LINPHONE_CORE_PATH}/coreapi/.libs/liblinphone.so.4.0.0
#	${LINPHONE_CORE_PATH}/mediastreamer2/src/.libs/libmediastreamer.so
#	${LINPHONE_CORE_PATH}/mediastreamer2/src/.libs/libmediastreamer.so.1
#	${LINPHONE_CORE_PATH}/mediastreamer2/src/.libs/libmediastreamer.so.1.0.0
#	${LINPHONE_CORE_PATH}/oRTP/src/.libs/libortp.so
#	${LINPHONE_CORE_PATH}/oRTP/src/.libs/libortp.so.8
#	${LINPHONE_CORE_PATH}/oRTP/src/.libs/libortp.so.8.0.0
)
INSTALL(PROGRAMS ${LINPHONE_LIBS_R} DESTINATION lib)

