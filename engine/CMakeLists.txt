cmake_minimum_required(VERSION 3.10)

project(Engine)

# Engine source files
set(PROJECT_SOURCE_FILES
    source/Engine.h
    source/Engine.cpp
    source/Application.h
    source/Application.cpp
    source/input/InputManager.h
    source/input/InputManager.cpp
    source/graphics/ShaderProgram.h
)

# Create the Engine library first
add_library(${PROJECT_NAME} ${PROJECT_SOURCE_FILES})

# Include engine source folder
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/source)

# Add GLFW
add_subdirectory(thirdparty/glfw-3.4 "${CMAKE_CURRENT_BINARY_DIR}/glfw_build")
target_link_libraries(${PROJECT_NAME} PUBLIC glfw)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/glfw-3.4/include)

# Add GLEW
set(BUILD_UTILS OFF CACHE BOOL "utilities" FORCE)
add_subdirectory(thirdparty/glew/build/cmake "${CMAKE_CURRENT_BINARY_DIR}/glew_build")
target_link_libraries(${PROJECT_NAME} PUBLIC glew_s)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/glew/include)
target_compile_definitions(${PROJECT_NAME} PUBLIC GLEW_STATIC)