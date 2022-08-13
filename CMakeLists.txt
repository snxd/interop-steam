cmake_minimum_required(VERSION 3.0)
cmake_policy(SET CMP0015 NEW)

include_directories(.)
include_directories(interop)

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

if(WIN32)
    list(APPEND STEAM_SRC windows/resource.rc)
    list(APPEND STEAM_HEADERS windows/resource.h)

    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

set(STEAMWORKS_INCLUDE_DIR steamworks)

file(GLOB STEAMWORKS_HEADERS
    "steamworks/*.h")

set(STEAMWORKS_LIBRARY steam_api)
if(WIN32)
    if(CMAKE_GENERATOR_PLATFORM MATCHES "Win32")
        list(APPEND STEAMWORKS_LIBRARY_DIRS steamworks/bin/win32)
    else()
        list(APPEND STEAMWORKS_LIBRARY_DIRS steamworks/bin/win64)
        set(STEAMWORKS_LIBRARY steam_api64)
    endif()
elseif(APPLE)
    set(STEAMWORKS_LIBRARY_DIRS steamworks/bin/osx32)
else()
    if(CMAKE_GENERATOR_PLATFORM MATCHES "x64")
        list(APPEND STEAMWORKS_LIBRARY_DIRS steamworks/bin/linux64)
    else()
        list(APPEND STEAMWORKS_LIBRARY_DIRS steamworks/bin/linux32)
    endif()
endif()

link_directories(${STEAMWORKS_LIBRARY_DIRS})
include_directories(${STEAMWORKS_INCLUDE_DIR})

source_group("Steamworks" FILES ${STEAMWORKS_HEADERS})

if(WIN32)
    set(CMAKE_C_FLAGS_DEBUG "/MTd")
    set(CMAKE_C_FLAGS_RELEASE "/MT")
    set(CMAKE_C_FLAGS_MINSIZEREL "/MT")
    set(CMAKE_C_FLAGS_RELWITHDEBINFO "/MT")

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
set_target_properties(${PROJECT_NAME} PROPERTIES POSITION_INDEPENDENT_CODE ON)
target_link_libraries(${PROJECT_NAME} interoplib ${STEAMWORKS_LIBRARY})

if(WIN32)
    if(CMAKE_GENERATOR_PLATFORM MATCHES "x64")
        set_target_properties(${PROJECT_NAME} PROPERTIES
            LINK_FLAGS "/DELAYLOAD:steam_api64.dll")
    else()
        set_target_properties(${PROJECT_NAME} PROPERTIES
            LINK_FLAGS "/DELAYLOAD:steam_api.dll")
    endif()
endif()