cmake_minimum_required(VERSION 3.30)
project(Maman14Project C)

set(CMAKE_C_STANDARD 11)

add_executable(Maman14Project
        main.c
        preProcessor.c
        errorHandler.c
        lineAnalyzer.c
        Validations.c
        objectTableAnalyzer.c
        second.c
        binaryConvertor.c
        utils.c
)

target_include_directories(Maman14Project PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})