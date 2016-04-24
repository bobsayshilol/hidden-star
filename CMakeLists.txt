cmake_minimum_required (VERSION 3.1)
project (hiddenstar)

# SDL2
find_package (PkgConfig REQUIRED)
pkg_check_modules (SDL2 REQUIRED sdl2)
pkg_check_modules (SDL2IMAGE REQUIRED SDL2_image>=2.0.0)
include_directories (${SDL2_INCLUDE_DIRS} ${SDL2IMAGE_INCLUDE_DIRS})

# hiddenstar files
add_library (combat combat.c)
add_library (comms comms.c)
add_library (gui gui.c)
add_library (intro intro.c)
add_library (planets planets.c)
add_library (travel travel.c)
add_executable (hiddenstar main.c)
set_property (TARGET hiddenstar PROPERTY C_STANDARD 99)
include_directories (util)
target_link_libraries (hiddenstar combat comms gui intro planets travel)
target_link_libraries (hiddenstar vector)
target_link_libraries (hiddenstar ${SDL2_LIBRARIES} ${SDL2IMAGE_LIBRARIES} m)

add_subdirectory (standalone)
add_subdirectory (util)

# Installation
install (TARGETS hiddenstar DESTINATION bin)
