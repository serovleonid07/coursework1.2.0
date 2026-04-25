cmake_minimum_required(VERSION 3.10)
project(coursework1.2.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SFML_ROOT "C:/msys64/ucrt64")

include_directories("${SFML_ROOT}/include")
link_directories("${SFML_ROOT}/lib")

add_executable(coursework1.2.0
    src/main.cpp
    src/GameLogic.cpp
    src/Render.cpp
    src/Menu.cpp
    src/History.cpp
)

target_link_libraries(coursework1.2.0
    sfml-graphics
    sfml-window
    sfml-system
)

# (Блок копирования удалён – DLL скопируем вручную)