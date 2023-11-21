##########
## init ##
##########
cmake_minimum_required(VERSION 3.23)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
project(cosmology)

############
## source ##
############
set(
        SOURCE_FILES
        src/cosmology.cpp src/cosmology.h
        src/paradigm.h)
add_library(cosmology STATIC ${SOURCE_FILES})
target_include_directories(cosmology PRIVATE src)
