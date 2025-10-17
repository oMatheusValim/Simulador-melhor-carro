# Cria uma biblioteca estática com os arquivos da ImGui
add_library(imgui STATIC
    imgui.cpp
    imgui_draw.cpp
    imgui_tables.cpp
    imgui_widgets.cpp
    imgui-SFML.cpp
)

# Informa ao CMake para incluir os cabeçalhos da pasta atual
target_include_directories(imgui PUBLIC .)

# Define o macro IMGUI_SFML_STATIC para compilar em modo estático
# Isso desativa a necessidade do arquivo "imgui-SFML_export.h"
target_compile_definitions(imgui PUBLIC IMGUI_SFML_STATIC)

# Informa à biblioteca 'imgui' que ela depende (linka com) da biblioteca 'sfml-graphics'
target_link_libraries(imgui PUBLIC sfml-graphics)