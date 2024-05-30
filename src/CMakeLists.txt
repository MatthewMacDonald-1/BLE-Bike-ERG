
# Main Program
set(MAIN_SRC 
    main.cpp
    
    font-settings.cpp
    font-settings.hpp
    video-settings.cpp
    video-settings.hpp
    scene.cpp
    scene.hpp
    main-menu.cpp
    main-menu.hpp

    MattsUtils/relative-drawing.cpp
    MattsUtils/relative-drawing.hpp
    MattsUtils/raylib-structs.cpp
    MattsUtils/raylib-structs.hpp
    MattsUtils/bounds.cpp
    MattsUtils/bounds.hpp
    MattsUtils/parsing.cpp
    MattsUtils/parsing.hpp
    MattsUtils/string-func.cpp
    MattsUtils/string-func.hpp
)

add_executable(BLE_BIKE_ERG ${MAIN_SRC})
target_link_libraries(BLE_BIKE_ERG simpleble::simpleble)
target_link_libraries(BLE_BIKE_ERG raylib)
set_target_properties(BLE_BIKE_ERG
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/../bin"
)
