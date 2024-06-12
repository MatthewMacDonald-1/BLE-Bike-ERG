
cmake_minimum_required (VERSION 3.21)
project (BLE_Bike_ERG)

set(CMAKE_C_STANDARD 11) # Requires C11 standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/lib/SimpleBLE/simpleble ${CMAKE_BINARY_DIR}/simpleble)

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/lib/raylib ${CMAKE_BINARY_DIR}/raylib)

include_directories(${CMAKE_CURRENT_LIST_DIR}/include)

# Main Program
set(MAIN_SRC 
    src/main.cpp
    src/config.hpp
    src/font-settings.cpp
    src/font-settings.hpp
    src/video-settings.cpp
    src/video-settings.hpp
    src/bluetooth-controller.cpp
    src/bluetooth-controller.hpp
    src/bluetooth-utils.cpp
    src/bluetooth-utils.hpp
    src/scene.cpp
    src/scene.hpp
    src/scene-manager.cpp
    src/scene-manager.hpp
    src/main-menu.cpp
    src/main-menu.hpp
    src/settings-menu.cpp
    src/settings-menu.hpp
    src/workout-scene.cpp
    src/workout-scene.hpp
    src/workout-selection-scene.cpp
    src/workout-selection-scene.hpp
    src/workout-definition.cpp
    src/workout-definition.hpp

    src/MattsUtils/relative-drawing.cpp
    src/MattsUtils/relative-drawing.hpp
    src/MattsUtils/raylib-structs.cpp
    src/MattsUtils/raylib-structs.hpp
    src/MattsUtils/bounds.cpp
    src/MattsUtils/bounds.hpp
    src/MattsUtils/number.cpp
    src/MattsUtils/number.hpp
    src/MattsUtils/time.cpp
    src/MattsUtils/time.hpp
    src/MattsUtils/string-func.cpp
    src/MattsUtils/string-func.hpp
)

add_executable(BLE_BIKE_ERG ${MAIN_SRC})
target_link_libraries(BLE_BIKE_ERG simpleble::simpleble)
target_link_libraries(BLE_BIKE_ERG raylib)
set_target_properties(BLE_BIKE_ERG
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/bin"
)


