# Not tested with versions below this.
cmake_minimum_required(VERSION 3.15)

project (liblsmt VERSION 0.1
              DESCRIPTION "LSM Tree implementation in C++20"
              LANGUAGES CXX)

# Adding INTERFACE implies no source files.
add_executable(lsmt src/main.cpp) 
target_include_directories(lsmt 
    PUBLIC
        # Generator expression to differentiate between building and installing locations.
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include> 
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)
# Add compile options
target_compile_options(lsmt PRIVATE -Werror)
target_compile_features(lsmt PRIVATE cxx_std_17)

# Tests follow similar target pattern, except we won't actually install it.
add_executable(test_lsmt test/test_lsmt.cpp) 
target_include_directories(test_lsmt
    PUBLIC
        # Generator expression to differentiate between building and installing locations.
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include> 
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)
# Add compile options
target_compile_options(test_lsmt PRIVATE -Werror)
target_compile_features(test_lsmt PRIVATE cxx_std_17)

set(CMAKE_CXX_COMPILER g++-9)
