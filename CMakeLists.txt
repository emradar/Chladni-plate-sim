cmake_minimum_required(VERSION 3.14)
project(chladni-plate-sim)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# SDL
add_subdirectory(vendored/SDL EXCLUDE_FROM_ALL)

# GLAD
set(GLAD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/glad")
file(GLOB GLAD_SOURCES "${GLAD_DIR}/*.c")
include_directories("${GLAD_DIR}/include")

file(GLOB_RECURSE SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/glad/glad.c"
)

add_executable(chladni-plate-sim ${SOURCES} ${GLAD_SOURCES})

target_include_directories(chladni-plate-sim PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    "${GLAD_DIR}/include"
)

find_package(OpenGL REQUIRED)
target_link_libraries(chladni-plate-sim PRIVATE SDL3::SDL3)

# Copy SDL3.dll to build output directory after build
add_custom_command(TARGET chladni-plate-sim POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    $<TARGET_FILE:SDL3::SDL3>
    $<TARGET_FILE_DIR:chladni-plate-sim>
    COMMENT "Copying SDL3.dll to output directory"
    VERBATIM
)
