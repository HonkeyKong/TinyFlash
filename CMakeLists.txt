# TinyFlash
# Author: Ryan Souders (honkeykong@honkeykong.org)
cmake_minimum_required(VERSION 3.13)

set(DEVKITPRO /opt/devkitpro)
set(CMAKE_TOOLCHAIN_FILE ${DEVKITPRO}/cmake/GBA.cmake)

project(TinyFlash C ASM)

set( SOURCE_FILES
    src/main.c
    src/text.c
    src/flash.c
    res/font.s
    res/rom.s )

include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable( tinyflash ${SOURCE_FILES} )

# Define the ROM binary and generated files
set(ROM_BINARY "${CMAKE_SOURCE_DIR}/res/rom.gba")
set(ROM_HEADER "${CMAKE_SOURCE_DIR}/res/rom.h")
set(ROM_ASM "${CMAKE_SOURCE_DIR}/res/rom.s")

# Add a custom command to run bin2s
add_custom_command(
    OUTPUT ${ROM_HEADER} ${ROM_ASM}
    COMMAND bin2s -H ${ROM_HEADER} ${ROM_BINARY} > ${ROM_ASM}
    DEPENDS ${ROM_BINARY} # Ensure the command runs when the ROM binary changes
    COMMENT "Converting ROM binary to header and assembly files..."
    VERBATIM
)

# Add a custom target to depend on the generated files
add_custom_target(generate_rom_files ALL
    DEPENDS ${ROM_HEADER} ${ROM_ASM}
)

# Include the generated header file in your project
include_directories(${CMAKE_BINARY_DIR})

# Make the executable depend on the custom target
add_dependencies(tinyflash generate_rom_files)

set_target_properties(tinyflash PROPERTIES GBA_MULTIBOOT TRUE)
set_target_properties(tinyflash PROPERTIES GBA_TITLE "TinyFlash")

gba_create_rom(tinyflash MULTIBOOT ${GBA_MULTIBOOT} TITLE ${GBA_TITLE})

# Uncomment this to launch your emulator after building.
# add_custom_command(TARGET tinyflash POST_BUILD
#     WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
#     COMMAND mGBA ./tinyflash.gba
#     COMMENT "Launching tinyflash in mGBA...")