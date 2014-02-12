project(cvvisual)
cmake_minimum_required(VERSION 2.8)

# we need C++11 and want warnings:
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra -pedantic")

# for some reason Qt requires this:
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")

# add some nice 'no-ugly-code-enforcers':
if(CMAKE_BUILD_TYPE MATCHES "Debug")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_GLIBCXX_DEBUG")
elseif(CMAKE_BUILD_TYPE MATCHES "Release")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")
endif()

# options
##################################################################

Option(CVV_DEBUG_MODE "cvvisual-debug-mode" ON)
Option(CVV_BUILD_TESTS "cvvisual-build-tests" ON)
Option(CVV_ASAN "cvvisual-asan" OFF)


#####################

if(CVV_DEBUG_MODE MATCHES ON)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DCVVISUAL_DEBUGMODE")
endif()

if(CVV_ASAN MATCHES ON)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fno-omit-frame-pointer")
endif()


# packages
##################################################################

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

if(CVV_BUILD_TESTS)
	enable_testing()
	find_package(GTest REQUIRED)
	include_directories(${GTEST_INCLUDE_DIR})
endif()

FIND_PACKAGE(Qt5Core REQUIRED)
include_directories(${Qt5Core_INCLUDE_DIRS})
FIND_PACKAGE(Qt5Widgets REQUIRED)
include_directories(${Qt5Widgets_INCLUDE_DIRS})
add_definitions(${QT_DEFINITIONS})
set(CMAKE_AUTOMOC ON)


# also: include our own headers
include_directories("${CMAKE_SOURCE_DIR}/include/opencv2")

# targets
##################################################################

file(GLOB implementation_sources "src/*/*.cpp")
file(GLOB implementation_headers "src/*/*.hpp")
file(GLOB api_headers "include/opencv2/*.hpp")
add_library( cvvisual
        ${implementation_sources}
        ${implementation_headers}
        ${api_headers}
)
target_link_libraries(cvvisual
	#Using those results in ugly problems with gtest:
	#${OpenCV_LIBS}
	opencv_core
	opencv_imgproc
	pthread             #Don't delete this line!!!
			    #Deleting it prevents the tests compiling on
			    #(X)Ubuntu 13.10!!!
	#Those do not work
	#${QT_LIBRARIES}
	Qt5Core
	Qt5Widgets
	Qt5Gui
	#without the following libraries it doesn't run on debian testing anymore
	m
	stdc++
)


if(CVV_BUILD_TESTS)
	file(GLOB test_sources "test/*.cpp")
	file(GLOB test_headers "test/*.hpp")
	add_executable(UnitTests
		${test_sources}
		${test_headers}
	)
	target_link_libraries(UnitTests
		cvvisual
		pthread
		gtest
	)
	add_test(
		NAME UnitTests
		COMMAND UnitTests
	)
	
	add_executable(test_manual_collapsable manual_test/collapsable/main.cpp)
	target_link_libraries(test_manual_collapsable cvvisual)
	
	add_executable(test_manual_filters manual_test/filters/main.cpp)
	target_link_libraries(test_manual_filters cvvisual opencv_highgui)
	
	add_executable(test_manual_accordion manual_test/accordion/main.cpp manual_test/accordion/acctester.cpp)
	target_link_libraries(test_manual_accordion cvvisual Qt5Core Qt5Widgets)

	add_executable(test_manual_signslot manual_test/signalslot/main.cpp)
	target_link_libraries(test_manual_signslot cvvisual Qt5Core Qt5Widgets)

	add_executable(test_manual_registerhelper manual_test/registerhelper/main.cpp )
	target_link_libraries(test_manual_registerhelper cvvisual Qt5Core Qt5Widgets Qt5Gui)

	add_executable(test_manual_fsw manual_test/fsw/main.cpp )
	target_link_libraries(test_manual_fsw cvvisual Qt5Core Qt5Widgets Qt5Gui)

	add_executable(test_manual_filtercalltab manual_test/filter_call_tab/main.cpp)
	target_link_libraries(test_manual_filtercalltab cvvisual Qt5Core Qt5Widgets)
	
	add_executable(test_manual_defaultfilterview manual_test/defaultfilterviewtest/main.cpp)
	target_link_libraries(test_manual_defaultfilterview cvvisual Qt5Core opencv_core)
	
	add_executable(test_manual_singlefilterview manual_test/singlefilterviewtest/main.cpp)
	target_link_libraries(test_manual_singlefilterview cvvisual Qt5Core opencv_core)

	add_executable(test_manual_matchscenetest manual_test/matchscenetest/main.cpp)
	target_link_libraries(test_manual_matchscenetest cvvisual Qt5Core opencv_highgui opencv_core)

	add_executable(test_manual_dualfilterview manual_test/dual_filter_view/main.cpp)
    	target_link_libraries(test_manual_dualfilterview cvvisual Qt5Core opencv_core opencv_highgui)
	
	add_executable(test_manual_dbg_printer manual_test/dbg_printer/main.cpp)
	target_link_libraries(test_manual_dbg_printer cvvisual Qt5Core)

	add_executable(test_manual_imageconvert manual_test/imageconvert/main.cpp)
	target_link_libraries(test_manual_imageconvert cvvisual Qt5Core Qt5Widgets opencv_highgui opencv_core opencv_imgproc)

	add_executable(test_manual_imageconvertperf manual_test/imageconvertperf/main.cpp)
	target_link_libraries(test_manual_imageconvertperf cvvisual Qt5Core Qt5Widgets opencv_core opencv_imgproc)

	add_executable(test_manual_zoomableimage manual_test/zoomableimage/main.cpp)
	target_link_libraries(test_manual_zoomableimage cvvisual Qt5Core Qt5Widgets opencv_highgui opencv_core opencv_imgproc)

	add_executable(test_manual_imageview manual_test/imageview/main.cpp)
	target_link_libraries(test_manual_imageview cvvisual Qt5Core Qt5Widgets opencv_core)
endif()
