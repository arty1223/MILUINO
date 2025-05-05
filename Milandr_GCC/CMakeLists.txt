cmake_minimum_required(VERSION 3.20)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_C_STANDARD 11)
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_VERSION 1)
# specify cross compilers and tools
set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER arm-none-eabi-g++)
set(CMAKE_ASM_COMPILER arm-none-eabi-gcc)
set(CMAKE_AR arm-none-eabi-ar)
set(CMAKE_OBJCOPY arm-none-eabi-objcopy)
set(CMAKE_OBJDUMP arm-none-eabi-objdump)
set(SIZE arm-none-eabi-size)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-register")
# project settings
project(mldr_test C CXX ASM)

add_compile_options(-mfloat-abi=soft)
add_compile_options(-mcpu=cortex-m3 -mthumb)
add_compile_options(-ffunction-sections -fdata-sections -fno-common -fmessage-length=0)
add_compile_options("$<$<COMPILE_LANGUAGE:CXX>:-fno-exceptions -fno-rtti>")


# path to spl/mdr32fxqi:
set(SPL_SRC_PATH "${CMAKE_SOURCE_DIR}/SPL/MDR32FxQI/src")
include_directories(
        "${CMAKE_SOURCE_DIR}/CMSIS/MDR32FxQI/CoreSupport/CM3"
        "${CMAKE_SOURCE_DIR}/CMSIS/MDR32FxQI/DeviceSupport/MDR32F9Q2I/inc"
        "${CMAKE_SOURCE_DIR}/CMSIS/MDR32FxQI/DeviceSupport/MDR32F9Q2I/startup"
        "${CMAKE_SOURCE_DIR}/SPL/MDR32FxQI/"
		"${CMAKE_SOURCE_DIR}/SPL/MDR32FxQI/inc"
		"${CMAKE_SOURCE_DIR}/App/Inc/"
)


set(SOURCES
    "${CMAKE_SOURCE_DIR}/startup_gcc_MDR32F9Q2I.s"
    "${CMAKE_SOURCE_DIR}/CMSIS/MDR32FxQI/DeviceSupport/MDR32F9Q2I/startup/system_MDR32F9Q2I.c"
    "${CMAKE_SOURCE_DIR}/App/Src/main.c"
	"${SPL_SRC_PATH}/MDR32FxQI_rst_clk.c"
	"${SPL_SRC_PATH}/MDR32FxQI_port.c"
)

set(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/MDR32F9Q2I.ld)

add_link_options(-Wl,-gc-sections,--print-memory-usage,-Map=${PROJECT_BINARY_DIR}/${PROJECT_NAME}.map)
add_link_options(-mcpu=cortex-m3 -mthumb -mthumb-interwork)
add_link_options(-T ${LINKER_SCRIPT})

add_executable(${PROJECT_NAME}.elf ${SOURCES} ${LINKER_SCRIPT})
target_compile_definitions(${PROJECT_NAME}.elf PRIVATE
    USE_MDR32F9Q2I
)
set_target_properties(${PROJECT_NAME}.elf PROPERTIES
EXPORT_COMPILE_COMMANDS ON
CXX_STANDARD 20 
CXX_STANDARD_REQUIRED True
)
# Generate .hex and .bin files
set(HEX_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.hex)
set(BIN_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.bin)
add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -Oihex $<TARGET_FILE:${PROJECT_NAME}.elf> ${HEX_FILE}
    COMMAND ${CMAKE_OBJCOPY} -Obinary $<TARGET_FILE:${PROJECT_NAME}.elf> ${BIN_FILE}
    COMMENT "Building ${HEX_FILE} Building ${BIN_FILE}"
)