cmake_minimum_required(VERSION 3.2)

# Set up project.
project(quad_encoder
        VERSION 1.0
        DESCRIPTION "A library for reading quadrature encoder sensors.")

# Create library.
add_library(quad_encoder_driver STATIC ${CMAKE_CURRENT_SOURCE_DIR}/src/driver.cpp)

# Set include directories for internal/external use.
target_include_directories(quad_encoder_driver PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)