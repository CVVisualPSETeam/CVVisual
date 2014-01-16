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


#####################

if(CVV_DEBUG_MODE MATCHES ON)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DCVVISUAL_DEBUGMODE")
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


# also: include our own headers:
include_directories("${CMAKE_SOURCE_DIR}/include/opencv2")

# targets
##################################################################

file(GLOB implementation_sources "src/*/*.cpp")
file(GLOB implementation_headers "src/*/*.hpp")
file(GLOB api_headers "include/opencv2/*.hpp")
file(GLOB qt_headers "include/Qt/*.h")
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
	#Those do not work
	#${QT_LIBRARIES}
	Qt5Core
	Qt5Widgets
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
	
	add_executable(collapsable_test manual_test/collapsable/main.cpp)
	target_link_libraries(collapsable_test cvvisual)
	
	add_executable(basic_dilate_test manual_test/basic_dilate/main.cpp)
	target_link_libraries(basic_dilate_test cvvisual)
	
	add_executable(test_maual_accordion manual_test/accordion/main.cpp manual_test/accordion/acctester.cpp)
	target_link_libraries(test_maual_accordion cvvisual Qt5Core Qt5Widgets)
	
endif()
