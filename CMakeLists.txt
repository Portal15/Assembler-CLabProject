cmake_minimum_required(VERSION 3.30)
project(Maman14Project)

set(CMAKE_CXX_STANDARD 20)

add_executable(Maman14Project
        main.c
        preProcessor.c
        errorHandler.c
        lineAnalyzer.c
        lineValidation.c
        utils.c
)