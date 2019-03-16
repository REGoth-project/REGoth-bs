cmake_minimum_required (VERSION 3.9.0)
project (REGoth)

include(FetchContent)

###############################################################################
#                                Configuration                                #
###############################################################################

set(REGOTH_BSF_SUBMODULE_PATH "" CACHE PATH "Path to bsf sources to add as cmake subdirectory")

# Set up binary output directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

###############################################################################
#                            Add external libraries                           #
###############################################################################

# Let CMake know where to find the Findbsf.cmake file (at current folder)
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

if(REGOTH_BSF_SUBMODULE_PATH)
  # Optionally include bsf as cmake submodule for easier development on bsf
  add_subdirectory(${REGOTH_BSF_SUBMODULE_PATH} bsf)
else()
  # Find bsf libraries and headers
  find_package(bsf REQUIRED)
endif()

# Find BsZenLib
set(BSZENLIB_SKIP_FIND_BSF ON)
add_subdirectory(lib/BsZenLib)

###############################################################################
#                               Add Samples, etc                              #
###############################################################################
add_subdirectory(docs-source)
add_subdirectory(src)
