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


#####################

if(CVV_DEBUG_MODE MATCHES ON)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DCVVISUAL_DEBUGMODE")
endif()


# packages
##################################################################

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

enable_testing()
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIR})

FIND_PACKAGE(Qt5Core REQUIRED)
include_directories(${Qt5Core_INCLUDE_DIRS})
set(CMAKE_AUTOMOC ON)


# also: include our own headers:
include_directories("${CMAKE_SOURCE_DIR}/include/opencv2")

# targets
##################################################################

file(GLOB implementation_sources "src/*.cpp")
file(GLOB implementation_headers "src/*.hpp")
file(GLOB api_headers "include/opencv2/*.hpp")
add_library( cvvisual
        ${implementation_sources}
        ${implementation_headers}
        ${api_headers}
)
target_link_libraries(cvvisual
	${OpenCV_LIBS}
	${QT_LIBRARIES}
)


file(GLOB test_sources "test/*.cpp")
file(GLOB test_headers "test/*.hpp")
add_executable(UnitTests
	${test_sources}
	${test_headers}
)
target_link_libraries(UnitTests
	cvvisual
	gtest
	gtest_main
)
add_test(
	NAME UnitTests
	COMMAND UnitTests
)
