project(aid-mapping-cli)
cmake_minimum_required(VERSION 3.5)



set(SRC
    main.cpp
)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})


############################################################################
set(Boost_USE_STATIC_LIBS ON)
find_package(Boost COMPONENTS program_options)
if(NOT Boost_FOUND)
    message(FATAL_ERROR "boost not found")
endif()

include_directories(${Boost_INCLUDE_DIRS})


#################################################################################
find_package(Soci COMPONENTS postgresql)
if(NOT SOCI_FOUND)
    message(FATAL_ERROR "soci not found")
endif()

include_directories(${SOCI_INCLUDE_DIRS})
link_directories(${SOCI_LIBRARY_DIR})


#################################################################################
execute_process(
    COMMAND git log -n1 --pretty=format:%H
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
    OUTPUT_VARIABLE "GIT_COMMIT"
    ERROR_QUIET
    OUTPUT_STRIP_TRAILING_WHITESPACE)
message(STATUS "Git commit: ${GIT_COMMIT}")

add_definitions(-DGIT_COMMIT=${GIT_COMMIT})



add_executable(${PROJECT_NAME} ${SRC})
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 14)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD_REQUIRED ON)

target_link_libraries(${PROJECT_NAME} aid-mapping aid-utils ${Boost_LIBRARIES} ${SOCI_LIBRARY} ${SOCI_postgresql_PLUGIN})
