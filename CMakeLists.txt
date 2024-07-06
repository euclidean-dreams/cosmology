##########
## init ##
##########
cmake_minimum_required(VERSION 3.18)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
project(cosmology)

############
## source ##
############
set(
        SOURCE_FILES
        src/axioms.h
        src/axioms/constants.h
        src/axioms/interfaces.cpp src/axioms/interfaces.h
        src/axioms/macros.h
        src/axioms/mathematics.cpp src/axioms/mathematics.h
        src/axioms/primitive.cpp src/axioms/primitive.h
        src/axioms/time.cpp src/axioms/time.h
        src/axioms/premise.h

        src/cosmology.cpp src/cosmology.h
        src/luon.h

        src/perception/equalizer.h
        src/perception/ring_buffer.h

        src/impressions/impression.h
        src/impressions/happy_pink_hearts.h
        src/impressions/workshop.h
        src/impressions/bubbles.h src/impressions/bubbles.cpp
        src/impressions/hymn.h
        src/impressions/tyr.h
        src/impressions/puff.h
        src/impressions/barometer.h
        src/impressions/feathered_flight.h
)
add_library(cosmology STATIC ${SOURCE_FILES})
target_include_directories(cosmology PUBLIC src)
