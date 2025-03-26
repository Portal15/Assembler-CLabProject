cmake_minimum_required(VERSION 3.30)
project(Maman14Project C)

set(CMAKE_C_STANDARD 11)

add_executable(Maman14Project
        main.c
        preProcessor.c
        errorHandler.c
        inputAnalyzer.c
        validationsChecker.c
        secondPass.c
        objectTableAnalyzer.c
        binaryConvertor.c
        createFiles.c
        utils.c
)

target_include_directories(Maman14Project PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})