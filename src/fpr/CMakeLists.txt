project(aid-fpr)
cmake_minimum_required(VERSION 3.5)



set(SRC
    ../math.hpp
    ../complex.hpp


    config.hpp

    cqt.hpp
    cqt.cpp

    cqt/level.hpp
    cqt/level.cpp

    timeGrid.hpp
    timeGrid.cpp

    valueGrid.hpp
    valueGrid.cpp

    resultMaker.hpp
    resultMaker.cpp
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})


############################################################################
set(Boost_USE_STATIC_LIBS ON)
find_package(Boost)
if(NOT Boost_FOUND)
    message(FATAL_ERROR "boost not found")
endif()

include_directories(${Boost_INCLUDE_DIRS})


add_library(${PROJECT_NAME} ${SRC})
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 14)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD_REQUIRED ON)
