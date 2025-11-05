cmake_minimum_required(VERSION 3.14)
project(chladni-plate-sim)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(vendored/SDL EXCLUDE_FROM_ALL)

file(GLOB_RECURSE SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

add_executable(chladni-plate-sim ${SOURCES})

target_include_directories(chladni-plate-sim PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(chladni-plate-sim PRIVATE SDL3::SDL3)
