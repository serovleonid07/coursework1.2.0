cmake_minimum_required(VERSION 3.10)
project(coursework1.2.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SFML_ROOT "C:/msys64/ucrt64")
include_directories("${SFML_ROOT}/include")
link_directories("${SFML_ROOT}/lib")

# Добавьте 'resources.rc' в этот список
set(SOURCES
    src/main.cpp
    src/GameLogic.cpp
    src/Render.cpp
    src/Menu.cpp
    src/History.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/resources.rc
)

add_executable(coursework1.2.0 ${SOURCES})

set_target_properties(coursework1.2.0 PROPERTIES OUTPUT_NAME "TagGame")

target_link_libraries(coursework1.2.0
    sfml-graphics
    sfml-window
    sfml-system
)

# (Ваш существующий блок копирования ресурсов из папки 111...)

# =====================================================
# КОПИРОВАНИЕ ФАЙЛОВ ИЗ ПАПКИ "111" В ПАПКУ СБОРКИ
# (шрифт, фоны, DLL)
# =====================================================
# Копирование ресурсов из папки 111 в папку сборки
set(ASSETS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/111")
if(EXISTS "${ASSETS_DIR}")
    file(GLOB RESOURCES "${ASSETS_DIR}/*.*")
    foreach(RES ${RESOURCES})
        add_custom_command(TARGET coursework1.2.0 POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${RES}"
                $<TARGET_FILE_DIR:coursework1.2.0>
            COMMENT "Copying ${RES}"
        )
    endforeach()
else()
    message(WARNING "Папка 111 не найдена, копирование ресурсов отключено")
endif()