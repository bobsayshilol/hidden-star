cmake_minimum_required (VERSION 3.1)
project (64)

# SDL2
find_package (PkgConfig REQUIRED)
pkg_check_modules (SDL2 REQUIRED sdl2)
pkg_check_modules (SDL2IMAGE REQUIRED SDL2_image>=2.0.0)
include_directories (${SDL2_INCLUDE_DIRS} ${SDL2IMAGE_INCLUDE_DIRS})

# 64 files
add_executable (64 64.c)
set_property (TARGET 64 PROPERTY C_STANDARD 99)
target_link_libraries (64 ${SDL2_LIBRARIES} ${SDL2IMAGE_LIBRARIES} m)

add_executable (intro intro.c)
set_property (TARGET intro PROPERTY C_STANDARD 99)
target_link_libraries (intro ${SDL2_LIBRARIES} ${SDL2IMAGE_LIBRARIES} m)

add_executable (planet planet.c)
set_property (TARGET planet PROPERTY C_STANDARD 99)
target_link_libraries (planet ${SDL2_LIBRARIES} ${SDL2IMAGE_LIBRARIES} m)

# Installation
install (TARGETS 64 DESTINATION bin)
