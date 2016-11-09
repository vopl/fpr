project(aid-mapping)
cmake_minimum_required(VERSION 3.5)



set(SRC
    ../math.hpp

    peer.hpp

    solver.hpp
    solver.cpp
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

find_package(Soci COMPONENTS postgresql)
if(NOT SOCI_FOUND)
    message(FATAL_ERROR "soci not found")
endif()

include_directories(${SOCI_INCLUDE_DIRS})
link_directories(${SOCI_LIBRARY_DIR})

add_library(${PROJECT_NAME} ${SRC})
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 14)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD_REQUIRED ON)
