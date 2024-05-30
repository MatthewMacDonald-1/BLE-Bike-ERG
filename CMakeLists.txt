
cmake_minimum_required (VERSION 3.21)
project (BLE_Bike_ERG)

set(CMAKE_C_STANDARD 11) # Requires C11 standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/lib/SimpleBLE/simpleble ${CMAKE_BINARY_DIR}/simpleble)

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/lib/raylib ${CMAKE_BINARY_DIR}/raylib)

include_directories(${CMAKE_CURRENT_LIST_DIR}/include)

add_subdirectory(src)

