cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

set(VCPKG_ROOT "c:\\dev\\orc\\dfir-orc\\external\\vcpkg")

include(vcpkg.cmake)
vcpkg_install(
    PATH "${VCPKG_ROOT}"
    PACKAGES boost-outcome
    ARCH x64
    USE_STATIC_CRT ON
)


project(outcome_test)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CRT_TYPE "/MT")
list(APPEND COMPILE_OPTIONS_DEBUG "${CRT_TYPE}d")
list(APPEND COMPILE_OPTIONS_RELEASE "${CRT_TYPE}")

foreach(OPTION IN ITEMS ${COMPILE_OPTIONS_DEBUG})
    add_compile_options($<$<CONFIG:DEBUG>:${OPTION}>)
endforeach()

foreach(OPTION IN ITEMS ${COMPILE_OPTIONS_RELEASE})
    add_compile_options($<$<CONFIG:RELEASE>:${OPTION}>)
    add_compile_options($<$<CONFIG:MINSIZEREL>:${OPTION}>)
    add_compile_options($<$<CONFIG:RELWITHDEBINFO>:${OPTION}>)
endforeach()

add_executable(outcome_test EXCLUDE_FROM_ALL main.cpp)

target_compile_definitions(outcome_test
    PRIVATE
        UNICODE
        NOMINMAX
)

find_package(Boost REQUIRED)
target_include_directories(outcome_test PUBLIC "${Boost_INCLUDE_DIR}")

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT outcome_test)
