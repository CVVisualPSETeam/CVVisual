project(cvvisual)
cmake_minimum_required(VERSION 2.8)

#we need C++11 and want warnings:
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra -pedantic" )

if(CMAKE_BUILD_TYPE MATCHES "Debug")
	set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}" )
elseif(CMAKE_BUILD_TYPE MATCHES "Release")
	set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror" )
endif()

include_directories("${CMAKE_SOURCE_DIR}/include/opencv2")

message("source-dir = ${CMAKE_SOURCE_DIR}")

file(GLOB implementation_files "src/*.cpp")
add_library( cvvisual
	${implementation_files}
	# cpp-files
)

target_link_libraries( cvvisual
	# opencv, qt...
)
