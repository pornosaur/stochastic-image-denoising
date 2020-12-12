cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON -O3)

project(core-image-denoising)

# LIBS
find_package( OpenCV REQUIRED )
find_package(OpenMP REQUIRED )

set(LIBS ${OpenCV_LIBS} -fopenmp tbb stdc++fs)




# Inlcude header files
include_directories(
        include
)

# Add subdir with sources
#add_subdirectory()

# Set sources
set(INCLUDE
        include/image_denoising.h
        )

set(SOURCES
        src/image_denoising.cpp

        # MAIN
        main.cpp

        )



add_library(${PROJECT_NAME}_lib STATIC ${SOURCES} ${INCLUDE})
set_target_properties(${PROJECT_NAME}_lib PROPERTIES POSITION_INDEPENDENT_CODE ON)
target_link_libraries(${PROJECT_NAME}_lib ${LIBS})

add_executable(${PROJECT_NAME} ${SOURCES} ${INCLUDE})
target_link_libraries(${PROJECT_NAME} PUBLIC ${LIBS})