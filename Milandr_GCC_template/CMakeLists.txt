cmake_minimum_required(VERSION 3.20)

include("cmake/gcc-milandr.cmake")
# project settings
set(CMAKE_PROJECT_NAME Milandr-template)
project(${CMAKE_PROJECT_NAME} ASM C CXX)
add_subdirectory(Drivers)

add_executable(${CMAKE_PROJECT_NAME})

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    "app/inc"
)

target_sources(${CMAKE_PROJECT_NAME} PRIVATE 
    "app/src/app.c"
	"app/src/clk.c"
	"app/src/systick.c"
)

# target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE
#     USE_MDR32F9Q2I
# )

# Add linked libraries
target_link_libraries(${CMAKE_PROJECT_NAME}
    milandr_sdk
)

add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${CMAKE_PROJECT_NAME}> ${CMAKE_PROJECT_NAME}.hex
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${CMAKE_PROJECT_NAME}> ${CMAKE_PROJECT_NAME}.bin
    COMMENT "Generating HEX and BIN files"
)