cmake_minimum_required(VERSION 3.17)
project(gTest01 C)

set(CMAKE_C_STANDARD 11)

##################################
#Have the font file in the resources folder
#within the build folder
##################################

#NOT optional
set(CMAKE_MODULE_PATH "C:/Dev/CPP/CMake/Modules") #path of FindSDL2.cmake

#NOT optional
find_package(SDL2 REQUIRED)

#doesn't seem to be required
#include_directories(${SDL2_INCLUDE_DIR})

set(SOURCE_FILES main_display.c)

add_executable(${CMAKE_PROJECT_NAME} ${SOURCE_FILES} )

target_link_libraries(${CMAKE_PROJECT_NAME} ${SDL2_LIBRARY} -lSDL2_ttf -lSDL2_image -mconsole) # -mconsole is needed to get console output