# Not tested with versions below this.
cmake_minimum_required(VERSION 3.15)

project (liblsmt VERSION 0.1
              DESCRIPTION "LSM Tree implementation in C++20"
              LANGUAGES CXX)

# LIBRARY
add_library(Lsmt INTERFACE)
target_include_directories(Lsmt
    INTERFACE 
        # Generator expression to differentiate between building and installing locations.
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include> 
)
# Add compile options
target_compile_options(Lsmt INTERFACE -Werror)
target_compile_features(Lsmt INTERFACE cxx_std_17)


# TESTING
# Prepare "Catch" library for other executables
set(CATCH_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include/catch2)
add_library(Catch INTERFACE)
target_include_directories(Catch INTERFACE ${CATCH_INCLUDE_DIR})
add_executable(test_lsmt test/test_lsmt.cpp)
target_link_libraries(test_lsmt Catch Lsmt)

enable_testing()
add_test(CatchTests test_lsmt)


# EXECUTABLE MAIN
add_executable(Main src/main.cpp) 
target_link_libraries(Main Lsmt)


set(CMAKE_CXX_COMPILER g++-9)
