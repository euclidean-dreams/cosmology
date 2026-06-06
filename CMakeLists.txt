##########
## init ##
##########
cmake_minimum_required(VERSION 3.18)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
project(cosmology)

###############
## threading ##
###############
find_package(Threads)
message("threads found?")
if (Threads_FOUND)
    message(STATUS "threads found!")
    set(threads_found ON)
else ()
    message(STATUS "threads unfounded")
    set(threads_found OFF)
endif ()


############
## source ##
############
set(
        SOURCE_FILES

        src/luon.h
        src/cosmology.cpp src/cosmology.h

        src/axioms.h
        src/axioms/constants.h
        src/axioms/interfaces.cpp src/axioms/interfaces.h
        src/axioms/macros.h
        src/axioms/mathematics.cpp src/axioms/mathematics.h
        src/axioms/primitive.cpp src/axioms/primitive.h
        src/axioms/time.cpp src/axioms/time.h
        src/axioms/premise.h

        src/perception/equalizer.h
        src/perception/ring_buffer.h

        src/palettes/centrifugal_palette.h
        src/palettes/damper_palette.h

        src/impressions/impression.h
        src/impressions/happy_pink_hearts.h
        src/impressions/workshop.h
        src/impressions/bubbles.h src/impressions/bubbles.cpp
        src/impressions/puff.h
        src/impressions/barometer.h
        src/impressions/calibration.h
        src/impressions/watercolor.h
        src/impressions/juice.h
        src/impressions/here_there_be_dragons.h
        src/impressions/obsidian.h
        src/impressions/curvature.h
        src/impressions/linework.h
        src/impressions/gnarl.h
        src/impressions/knit.h
        src/impressions/meshwork.h
        src/impressions/spherics.h
        src/impressions/ambiance.h
        src/impressions/allegory/allegory.h
        src/impressions/allegory/allegorion/allegorion.h
)
if (threads_found)
    set(
            src/impressions/hymn.h
            src/impressions/tyr.h
    )
endif ()

if (kfr)
    message(STATUS "fft's with be performed with kfr")
    find_package(KFR CONFIG REQUIRED)
    set(
            SOURCE_FILES
            ${SOURCE_FILES}
            src/perception/kfr_fft.h
    )
else ()
    message(STATUS "fft's with be performed with kissfft")
    SET(KISSFFT_TOOLS OFF)
    SET(KISSFFT_TEST OFF)
    SET(KISSFFT_STATIC ON)
    add_subdirectory(lib/kissfft)
    set(
            SOURCE_FILES
            ${SOURCE_FILES}
            src/perception/kiss_fft.h
    )
endif ()

add_library(cosmology STATIC ${SOURCE_FILES})
target_include_directories(cosmology PUBLIC src)

add_library(cosmology_selections INTERFACE)
if (threads_found)
    target_compile_definitions(cosmology_selections INTERFACE threads_found)
endif ()
target_link_libraries(cosmology PRIVATE cosmology_selections)

if (kfr)
    target_link_libraries(euclid PRIVATE kfr kfr_dft)
else ()
    target_link_libraries(cosmology PRIVATE kissfft)
endif ()
