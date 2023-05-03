cmake_minimum_required(VERSION 3.22)
project(WorldGame)

set(CMAKE_CXX_STANDARD 20)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

include_directories(include external/include)

set(EXTERNAL_SRC external/glad.c)
file(GLOB SOURCES src/*.cpp)

add_executable(${PROJECT_NAME} ${SOURCES} ${EXTERNAL_SRC})
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES})

if (APPLE)
    target_link_libraries(${PROJECT_NAME} "-framework IOKit")
    target_link_libraries(${PROJECT_NAME} "-framework Cocoa")
    target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
endif()

function(copy_resources)
    foreach(arg IN LISTS ARGN)
        configure_file(${CMAKE_CURRENT_SOURCE_DIR}/${arg} ${CMAKE_CURRENT_BINARY_DIR}/${arg} COPYONLY)
    endforeach()
endfunction()
copy_resources( res/default_shader.frag
                res/default_shader.vert
                res/grass.png)