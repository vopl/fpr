project(aid-utils)
cmake_minimum_required(VERSION 3.5)



set(SRC
    wavStore.hpp
    wavStore.cpp
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})



add_library(${PROJECT_NAME} ${SRC})
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 14)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD_REQUIRED ON)
