cmake_minimum_required(VERSION 3.15)
cmake_policy(SET CMP0015 NEW)

add_subdirectory(lib/interop)

set(STEAM_SRCS
    interop.c
    steamapi.h
    steamapi.cc
    steamapps.h
    steamapps.cc
    steamfriends.h
    steamfriends.cc
    steamuser.h
    steamuser.cc
    steamuserstats.h
    steamuserstats.cc
    steamutils.h
    steamutils.cc)
if(WIN32)
    list(APPEND STEAM_SRCS
        windows/resource.h
        windows/resource.rc)

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
    if(CMAKE_C_FLAGS MATCHES "-m32")
        list(APPEND STEAMWORKS_LIBRARY_DIRS steamworks/bin/linux32)
    else()
        list(APPEND STEAMWORKS_LIBRARY_DIRS steamworks/bin/linux64)
    endif()
endif()

source_group("Steamworks" FILES ${STEAMWORKS_HEADERS})

project(steam C CXX)

add_library(steam SHARED ${STEAM_SRCS} ${STEAMWORKS_HEADERS})
set_target_properties(steam PROPERTIES POSITION_INDEPENDENT_CODE ON)
target_link_libraries(steam interoplib ${STEAMWORKS_LIBRARY})
target_link_directories(steam PRIVATE ${STEAMWORKS_LIBRARY_DIRS})
target_include_directories(steam PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${STEAMWORKS_INCLUDE_DIR})
target_compile_definitions(steam PRIVATE $<$<BOOL:WIN32>:_CRT_SECURE_NO_WARNINGS>)

if(MSVC)
    if(CMAKE_GENERATOR_PLATFORM MATCHES "Win32")
        target_link_options(steam PRIVATE
            "/DELAYLOAD:steam_api.dll")
    else()
        target_link_options(steam PRIVATE
            "/DELAYLOAD:steam_api64.dll")
    endif()
    target_link_options(steam PRIVATE
        "/DEF:\"${CMAKE_CURRENT_SOURCE_DIR}/windows/steam.def\"")
elseif(APPLE)
    set_target_properties(steam PROPERTIES
        XCODE_ATTRIBUTE_EXPORTED_SYMBOLS_FILE "${CMAKE_CURRENT_SOURCE_DIR}/mac/steam.exp")
else()
    target_link_options(steam PRIVATE
        "-Wl,--version-script=${CMAKE_CURRENT_SOURCE_DIR}/linux/steam.map"
        "-Wl,--gc-sections")
endif()
