## CMAKE INITIALISATION
PROJECT(LaserRanger)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

SET(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_CURRENT_SOURCE_DIR}/cmake/cmake_modules")
## END CMAKE INITIALISATION

## PROJECT LANGUAGE
#ENABLE_LANGUAGE(CXX C)
## END PROJECT LANGUAGE

#if(NOT Boost_FOUND)
#	set(Boost_USE_MULTITHREADED ON)		# default is ON, use -mt variant
#	set(Boost_USE_STATIC_LIBS ON)		# default is OFF, use boost static library
#	set(Boost_USE_STATIC_RUNTIME ON)	# default is not set.
#
#	find_package(Boost 1.50.0 COMPONENTS serialization thread system chrono)
#	if(NOT Boost_FOUND)
#		set(BOOST_ROOT "C:/Program Files/boost/boost_1_50_0")		# if not set, it will let cmake to find it.
#		set(BOOST_LIBRARYDIR ${BOOST_ROOT}/stage/lib/x86)			# might need to set this too.
#		set(Boost_NO_SYSTEM_PATHS ON)								# default is OFF, set to ON if BOOST_ROOT is set
#		find_package(Boost 1.50.0 COMPONENTS serialization thread system chrono REQUIRED)
#	endif()
#endif()

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)

# Don't warn about zero-length format strings, which we sometimes
# use when constructing error objectss.
# SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D__STDC_CONSTANT_MACROS")
#SET (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-format-zero-length")

## PROJECT INCUDES DIRECTORIES

#LINK_DIRECTORIES(/$ENV{HOME}/ffmpeg_build/lib/)
#LINK_DIRECTORIES(/usr/lib/x86_64-linux-gnu/)

#SET(FFMPEG_INCLUDE_DIR /$ENV{HOME}/ffmpeg_build/include/)

## PROJCET INCLUDES DIRECTORIES
#INCLUDE_DIRECTORIES(${FFMPEG_INCLUDE_DIR})


INCLUDE_DIRECTORIES(include)
SET(SOURCES
 ${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
 ${CMAKE_CURRENT_LIST_DIR}/src/laser_ranging.cpp
 ${CMAKE_CURRENT_LIST_DIR}/src/view_frame.cpp
 ${CMAKE_CURRENT_LIST_DIR}/src/algorithm.cpp
 ${CMAKE_CURRENT_LIST_DIR}/src/capture_frame.cpp
 ${CMAKE_CURRENT_LIST_DIR}/src/timer.cpp
 ${CMAKE_CURRENT_LIST_DIR}/src/image_processing.cpp)


ADD_EXECUTABLE(LaserRanging ${SOURCES})

#TARGET_LINK_LIBRARIES(vision avdevice avfilter swscale avformat avcodec avutil swresample m va va-x11 pthread vorbis vpx opus z postproc dl fdk-aac mp3lame vorbisenc vorbisfile x264 theora x265 theoraenc theoradec dl rt bz2 asound SDL SDLmain SDL_ttf freetype ass lzma ftgl perl crypto xcb xcb-shm xcb-xfixes ao xcb-shape fftw3 X11 vdpau drm va-drm numa log4cpp)
#target_link_libraries(vision ${Boost_LIBRARIES} ${OpenCV_LIBS})
#target_link_libraries(vision ${Boost_LIBRARIES})


##TARGET_LINK_LIBRARIES(vision avdevice avfilter swscale avformat avcodec avutil swresample m va pthread vorbis vpx opus z postproc dl fdk-aac mp3lame vorbisenc vorbisfile x264 theora x265 theoraenc theoradec dl rt bz2 asound SDL SDLmain SDL_ttf freetype ass lzma ftgl perl crypto xcb xcb-shm xcb-xfixes ao xcb-shape fftw3 boost_system)

#set(SOURCES ${CMAKE_CURRENT_LIST_DIR}/src/server_vision.cpp)

#add_executable(exe ${SOURCES})
#TARGET_LINK_LIBRARIES(exe pthread )
#target_link_libraries(exe ${Boost_LIBRARIES})
target_link_libraries( LaserRanging ${OpenCV_LIBS} )

message( "*************************************" )
message( "*************************************" )
message( "*********** Laser Ranging ***********" )
message( "*************************************" )
message( "*************************************" )