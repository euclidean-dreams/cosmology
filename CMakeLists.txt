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
        src/axioms.h
        src/axioms/paradigm.h
        src/axioms/interfaces.h
        src/axioms/primitive.h
        src/cosmology.cpp include/cosmology.h
)
add_library(cosmology STATIC ${SOURCE_FILES})
target_include_directories(cosmology PRIVATE src)
target_include_directories(cosmology PUBLIC include)
