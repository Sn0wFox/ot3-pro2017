cmake_minimum_required(VERSION 3.5)
project(pdc_evol_model)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules" ${CMAKE_MODULE_PATH})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g")

find_package(OpenMP)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()


set(SOURCE_FILES main.cpp src/World.cpp src/World.h src/GridCell.cpp src/GridCell.h src/Organism.cpp src/Organism.h src/DNA.cpp src/DNA.h src/BP.cpp src/BP.h src/BP_Protein_Block.cpp src/BP_Protein_Block.h src/BP_Protein.cpp src/BP_Protein.h src/BP_Pump_Block.cpp src/BP_Pump_Block.h src/BP_Pump.cpp src/BP_Pump.h src/BP_Move_Block.cpp src/BP_Move_Block.h src/BP_Move.cpp src/BP_Move.h src/RNA.cpp src/RNA.h src/Protein.cpp src/Protein.h src/Move.cpp src/Move.h src/Pump.cpp src/Pump.h src/Common.cpp src/Common.h)

add_executable(pdc_evol_model ${SOURCE_FILES})

if(WITH_GRAPHICS_CONTEXT)
    find_package(SFML 2 REQUIRED graphics window system)
    if(SFML_FOUND)
        include_directories(${SFML_INCLUDE_DIR})
        target_link_libraries(pdc_evol_model ${SFML_LIBRARIES})
    endif(SFML_FOUND)
    list(APPEND eye_evolution SOURCES src/GraphicDisplay.cpp src/GraphicDisplay.h)
    add_definitions(-DWITH_GRAPHICS_CONTEXT=1)
endif(WITH_GRAPHICS_CONTEXT)


#find_package(ZLIB REQUIRED)
#if(ZLIB_FOUND)
#    include_directories(${ZLIB_INCLUDE_DIR})
#    target_link_libraries(pdc_evol_model ${ZLIB_LIBRARIES})
#endif(ZLIB_FOUND)