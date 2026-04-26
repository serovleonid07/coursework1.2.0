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

set(ASSETS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/111")

if(EXISTS "${ASSETS_DIR}")
    # Копируем шрифт (если есть)
    if(EXISTS "${ASSETS_DIR}/arial.ttf")
        add_custom_command(TARGET coursework1.2.0 POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${ASSETS_DIR}/arial.ttf"
                $<TARGET_FILE_DIR:coursework1.2.0>
            COMMENT "Копирование шрифта из папки 111"
        )
    endif()

    # Копируем все DLL из папки 111
    file(GLOB DLL_FILES "${ASSETS_DIR}/*.dll")
    foreach(DLL ${DLL_FILES})
        add_custom_command(TARGET coursework1.2.0 POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${DLL}"
                $<TARGET_FILE_DIR:coursework1.2.0>
            COMMENT "Копирование ${DLL}"
        )
    endforeach()
else()
    message(WARNING "Папка 111 не найдена, копирование файлов пропущено")
endif()

if(EXISTS "${ASSETS_DIR}/background.png")
    add_custom_command(TARGET coursework1.2.0 POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${ASSETS_DIR}/background.png"
            $<TARGET_FILE_DIR:coursework1.2.0>
        COMMENT "Copying background.png"
    )
endif()

if(EXISTS "${ASSETS_DIR}/backgroundh.png")
    add_custom_command(TARGET coursework1.2.0 POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${ASSETS_DIR}/backgroundh.png"
            $<TARGET_FILE_DIR:coursework1.2.0>
        COMMENT "Copying backgroundh.png"
    )
endif()