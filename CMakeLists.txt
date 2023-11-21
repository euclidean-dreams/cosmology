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
        include/axioms.h
        src/axioms/interfaces.h
        src/axioms/macros.h
        src/axioms/math.h
        src/axioms/primitive.h
        src/axioms/time.h
        src/cosmology.cpp include/cosmology.h
)
add_library(cosmology STATIC ${SOURCE_FILES})
target_include_directories(cosmology PRIVATE src)
target_include_directories(cosmology PUBLIC include)
