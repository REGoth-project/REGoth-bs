add_library(samples-common
  common/BsCameraZoomer.cpp
  common/BsFPSCamera.cpp
  common/BsFPSWalker.cpp
  common/BsObjectRotator.cpp
 )

target_include_directories(samples-common PUBLIC common)
target_link_libraries(samples-common PUBLIC bsf BsZenLib)

add_executable(REGoth main.cpp)
target_link_libraries(REGoth samples-common)
