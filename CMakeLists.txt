cmake_minimum_required (VERSION 3.1)
project (hiddenstar)

# SDL2
find_package (PkgConfig REQUIRED)
pkg_check_modules (SDL2 REQUIRED sdl2)
pkg_check_modules (SDL2IMAGE REQUIRED SDL2_image>=2.0.0)
include_directories (${SDL2_INCLUDE_DIRS} ${SDL2IMAGE_INCLUDE_DIRS})

# hiddenstar files
add_library (combat combat.c)
add_library (intro intro.c)
add_library (planets planets.c)
add_executable (hiddenstar main.c)
set_property (TARGET hiddenstar PROPERTY C_STANDARD 99)
target_link_libraries (hiddenstar combat intro planets ${SDL2_LIBRARIES} ${SDL2IMAGE_LIBRARIES} m)

add_executable (standalone/planet standalone/planet.c)
set_property (TARGET planet PROPERTY C_STANDARD 99)
target_link_libraries (planet ${SDL2_LIBRARIES} ${SDL2IMAGE_LIBRARIES} m)

add_executable (standalone/64 standalone/64.c)
set_property (TARGET 64 PROPERTY C_STANDARD 99)
target_link_libraries (64 ${SDL2_LIBRARIES} ${SDL2IMAGE_LIBRARIES} m)

# Installation
install (TARGETS hiddenstar DESTINATION bin)
install (TARGETS planet DESTINATION bin)
