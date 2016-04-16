cmake_minimum_required (VERSION 3.1)
project (hiddenstar/standalone)

add_executable (planet planet.c)
set_property (TARGET planet PROPERTY C_STANDARD 99)
target_link_libraries (planet ${SDL2_LIBRARIES} ${SDL2IMAGE_LIBRARIES} m)
