cmake_minimum_required(VERSION 3.20)

include("cmake/gcc-milandr.cmake")
# project settings
set(CMAKE_PROJECT_NAME Milandr-template)
project(${CMAKE_PROJECT_NAME} ASM C CXX)
add_subdirectory(Drivers)

add_executable(${CMAKE_PROJECT_NAME})

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    "App/inc"
)

target_sources(${CMAKE_PROJECT_NAME} PRIVATE 
    "App/src/app.c"
)

target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE
    USE_MDR32F9Q2I
)

# Add linked libraries
target_link_libraries(${CMAKE_PROJECT_NAME}
    milandr_sdk
)
