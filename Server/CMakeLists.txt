cmake_minimum_required(VERSION 3.10)

project(Server)

set(CMAKE_C_STANDARD 11)

set(SOURCE_FILES
    Network.h
    Server.h
    Errors.h
    main.c)

add_executable(server ${SOURCE_FILES})