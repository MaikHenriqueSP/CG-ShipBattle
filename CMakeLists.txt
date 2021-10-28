project(atividade1)

add_executable(${PROJECT_NAME} 
    main.cpp 
    openglwindow.cpp 
    ./bullets/bullets.cpp 
    ./ship/ship.cpp 
    starlayers.cpp 
    ./enemy/enemy_ship.cpp 
    ./util/util.cpp 
    ./base_ship/base_ship.cpp
)

enable_abcg(${PROJECT_NAME})