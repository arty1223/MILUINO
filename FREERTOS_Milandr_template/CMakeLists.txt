cmake_minimum_required(VERSION 3.20)

include("cmake/gcc-milandr.cmake")
# project settings
set(CMAKE_PROJECT_NAME FREERTOS-Milandr-template)
project(${CMAKE_PROJECT_NAME} ASM C CXX)
add_subdirectory(Drivers)

add_executable(${CMAKE_PROJECT_NAME})

# set_target_properties(${PROJECT_NAME} PROPERTIES
# EXPORT_COMPILE_COMMANDS ON
# CXX_STANDARD 20 
# CXX_STANDARD_REQUIRED True
# )

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    "app/inc"
	"FreeRTOS/include"
	"FreeRTOS/portable/GCC/ARM_CM3"
)

target_sources(${CMAKE_PROJECT_NAME} PRIVATE 
    # Project sources, User libraries
	"app/src/app.c"
	"app/src/clk.c"

	# FreeRTOS sources
	"FreeRTOS/croutine.c"
	"FreeRTOS/event_groups.c"
	"FreeRTOS/list.c"
	"FreeRTOS/queue.c"
	"FreeRTOS/stream_buffer.c"
	"FreeRTOS/tasks.c"
	"FreeRTOS/timers.c"
	
	# FreeRTOS portable sources
	"FreeRTOS/portable/GCC/ARM_CM3/port.c"
	"FreeRTOS/portable/MemMang/heap_4.c"
)

# target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE
#     USE_MDR32F9Q2I
# )

# Add linked libraries
target_link_libraries(${CMAKE_PROJECT_NAME}
    milandr_sdk
)

# add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD // генерация hex и bin файлов
#     COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${CMAKE_PROJECT_NAME}> ${CMAKE_PROJECT_NAME}.hex
#     COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${CMAKE_PROJECT_NAME}> ${CMAKE_PROJECT_NAME}.bin
#     COMMENT "Generating HEX and BIN files"
# )