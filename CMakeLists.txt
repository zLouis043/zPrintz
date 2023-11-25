cmake_minimum_required(VERSION 3.26.4)
project(ZI_PRINTF)
# Add executable target with source files listed in SOURCE_FILES variable
add_executable(${PROJECT_NAME} main.c  
                               src/printz.h            ) 
