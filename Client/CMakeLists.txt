cmake_minimum_required(VERSION 3.10)

project(Client)

set(CMAKE_C_STANDARD 11)

set(SOURCE_FILES
    Client.h
    Network.h
    Errors.h
    main.c)

add_executable(client ${SOURCE_FILES})