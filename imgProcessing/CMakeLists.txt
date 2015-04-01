cmake_minimum_required(VERSION 2.8)

project(Strabic-imgProcessing)

find_package(OpenCV REQUIRED)
find_package(Boost REQUIRED COMPONENTS system filesystem regex)
if(Boost_FOUND)
	include_directories(SYSTEM ${Boost_INCLUDE_DIR})
endif()

set(${PROJECT_NAME}_CPP imgProcessing-strabic.cpp)
set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")

add_executable(${PROJECT_NAME} ${${PROJECT_NAME}_CPP})
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS} ${Boost_LIBRARIES} curl curlpp)
