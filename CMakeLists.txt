cmake_minimum_required(VERSION 2.8)

include_directories(.)
include_directories(interop)
include_directories(steamworks)

add_subdirectory(interop)

set(STEAM_SRC
    steamapi.cpp
    steamapps.cpp
    steamfriends.cpp
    steaminterop.c
    steamuser.cpp
    steamuserstats.cpp
    steamutils.cpp)
set(STEAM_HEADERS
    steamapi.h
    steamapps.h
    steamfriends.h
    steamuser.h
    steamuserstats.h
    steamutils.h)

file(GLOB STEAMWORKS_HEADERS
    "steamworks/*.h")

if(WIN32)
    link_directories(${CMAKE_CURRENT_SOURCE_DIR}/bin)
    if(CMAKE_GENERATOR_PLATFORM MATCHES "x64")
        link_directories(${CMAKE_CURRENT_SOURCE_DIR}/bin/win64)
    else()
        link_directories(${CMAKE_CURRENT_SOURCE_DIR}/bin/win32)
    endif()
elseif(APPLE)
    link_directories(${CMAKE_CURRENT_SOURCE_DIR}/bin/osx32)
else()
    if(CMAKE_GENERATOR_PLATFORM MATCHES "x64")
        link_directories(${CMAKE_CURRENT_SOURCE_DIR}/bin/linux64)
    else()
        link_directories(${CMAKE_CURRENT_SOURCE_DIR}/bin/linux32)
    endif()
endif()

if(WIN32)
    list(APPEND STEAM_SRC windows/resource.rc)
    list(APPEND STEAM_HEADERS windows/resource.h)

    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

source_group("Steamworks" FILES ${STEAMWORKS_HEADERS})

if(WIN32)
    set(CMAKE_SHARED_LINKER_FLAGS
        "${CMAKE_MODULE_LINKER_FLAGS} \"/DEF:${CMAKE_CURRENT_SOURCE_DIR}/windows/steam.def\"")
elseif(APPLE)
    set(CMAKE_XCODE_ATTRIBUTE_EXPORTED_SYMBOLS_FILE
        "${CMAKE_CURRENT_SOURCE_DIR}/mac/steam.def")
elseif(UNIX)
    set(CMAKE_SHARED_LINKER_FLAGS 
        "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--version-script=${CMAKE_CURRENT_SOURCE_DIR}/linux/steam.def")
    set(CMAKE_SHARED_LINKER_FLAGS 
        "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--gc-sections")
endif()

project(steam C CXX)

add_library(${PROJECT_NAME} SHARED ${STEAM_SRC} ${STEAM_HEADERS} ${STEAMWORKS_HEADERS})
target_link_libraries(${PROJECT_NAME} interoplib steam_api)

if(WIN32)
    set_target_properties(${PROJECT_NAME} PROPERTIES 
        LINK_FLAGS "/DELAYLOAD:steam_api.dll;steam_api64.dll")
endif()