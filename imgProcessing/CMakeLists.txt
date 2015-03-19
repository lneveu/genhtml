cmake_minimum_required(VERSION 2.8)

project(Strabic-imgProcessing)

find_package(OpenCV REQUIRED)

set(${PROJECT_NAME}_CPP imgProcessing-strabic.cpp)

add_executable(${PROJECT_NAME} ${${PROJECT_NAME}_CPP})

target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS} curl curlpp)
