# SPDX-License-Identifier: Apache-2.0
cmake_minimum_required(VERSION 3.20)

project(debugger LANGUAGES CXX)

add_executable(${PROJECT_NAME})

target_sources(
    ${PROJECT_NAME}
    PRIVATE src/main.cpp include/DigiMouse.h
    ${PROJECT_SOURCE_DIR}/../src/main.cpp
)

target_include_directories(
    ${PROJECT_NAME}
    PRIVATE $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    PRIVATE $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/../include>
)

target_compile_definitions(
    ${PROJECT_NAME}
    PRIVATE "$<$<CXX_COMPILER_ID:MSVC>:NOMINMAX>" "$<$<CXX_COMPILER_ID:MSVC>:BUILD_FOR_WINDOWS>"
)

target_compile_features(
    ${PROJECT_NAME}
    PUBLIC cxx_std_20
)

install(TARGETS ${PROJECT_NAME})
set(CPACK_PACKAGE_NAME "${PROJECT_NAME}-${GIT_HASH}")
set(CPACK_PACKAGE_VENDOR "shigeno-y")
set(CPACK_PACKAGE_HOMEPAGE_URL "https://github.com/shigeno-y/ltc-generator")
include(CPack)

if(MSVC)
    set_property(DIRECTORY ${CMAKE_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
endif()
