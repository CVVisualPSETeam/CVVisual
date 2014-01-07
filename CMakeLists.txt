project(cvvisual)
cmake_minimum_required(VERSION 2.8)

#we need C++11 and want warnings:
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra -pedantic" )

if(CMAKE_BUILD_TYPE MATCHES "Debug")
	set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}" )
elseif(CMAKE_BUILD_TYPE MATCHES "Release")
	set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror" )
endif()

# packages
##################################################################

find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

enable_testing()
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIR})

include_directories("${CMAKE_SOURCE_DIR}/include/opencv2")

# targets
##################################################################

file(GLOB implementation_files "src/*.cpp")
add_library( cvvisual
	${implementation_files}
	# cpp-files
)

target_link_libraries( cvvisual
	${OpenCV_LIBS}
	# opencv, qt...
)

file(GLOB test_files "test/*.cpp")
add_executable(runUnitTests ${test_files})
target_link_libraries(runUnitTests cvvisual gtest gtest_main)

add_test(
    NAME runUnitTests
    COMMAND runUnitTests
)

