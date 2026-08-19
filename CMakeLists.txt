cmake_minimum_required(VERSION 4.1)
project(srenderer C)

set(CMAKE_C_STANDARD 11)

# Find the SDL3 package
find_package(SDL3 REQUIRED)


add_executable(c main.c
        sdl_init.h
        sdl_init.c
        draw.h
        draw.c
        vec.h
        vec.c
        mat.c
        mat.h
        constant.h
        mesh.h
        mesh.c
        texture.h
        textue.c
)

target_link_libraries(c PRIVATE
        SDL3::SDL3
        m
)
