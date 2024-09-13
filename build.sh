#!/bin/bash

g++ -std=gnu++20 -fcolor-diagnostics -fansi-escape-codes -g \
    src/main.cpp src/glad.c      \
    -o build/main_mac \
    -lglfw -lvulkan -framework OpenGL \
    -L/usr/local/lib -Wl,-rpath,/usr/local/lib \
    -I/usr/local/include -Iinclude