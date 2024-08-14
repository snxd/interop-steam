cmake_minimum_required(VERSION 3.15)

include_directories(.)

set(INTEROPLIB_SRC
    interoplib.c)

set(INTEROPLIB_HEADERS
    dictionaryi.h
    interoplib.h
    interopstub.h
    taski.h
    workflowi.h)

project(interoplib C)

add_library(${PROJECT_NAME} STATIC ${INTEROPLIB_SRC} ${INTEROPLIB_HEADERS})
set_property(TARGET ${PROJECT_NAME} PROPERTY POSITION_INDEPENDENT_CODE ON)