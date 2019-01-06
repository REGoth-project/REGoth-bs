cmake_minimum_required (VERSION 3.9.0)
project (REGoth)

# Set up binary output directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)


###############################################################################
#                            Add external libraries                           #
###############################################################################

# Let CMake know where to find the Findbsf.cmake file (at current folder)
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

# Find bsf libraries and headers
find_package(bsf REQUIRED)


###############################################################################
#                               Add Samples, etc                              #
###############################################################################
add_subdirectory(docs-source)
