include(FetchContent)

# GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.4
    GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(glfw)

# Assimp
set(ASSIMP_BUILD_IFC_IMPORTER OFF CACHE BOOL "Disable IFC" FORCE)
set(ASSIMP_BUILD_STEP_IMPORTER OFF CACHE BOOL "Disable STEP" FORCE)
set(ASSIMP_BUILD_GLTF_IMPORTER OFF CACHE BOOL "Disable GLTF" FORCE)
set(ASSIMP_BUILD_COLLADA_IMPORTER OFF CACHE BOOL "Disable Collada" FORCE)
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "Disable tests" FORCE)
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "Disable tools" FORCE)
set(ASSIMP_INSTALL OFF CACHE BOOL "Disable install targets" FORCE)
set(HUNTER_ENABLED OFF CACHE BOOL "Disable Hunter package manager" FORCE)

FetchContent_Declare(
    assimp
    GIT_REPOSITORY https://github.com/assimp/assimp.git
    GIT_TAG v6.0.5
)

FetchContent_MakeAvailable(assimp)

file(
    GLOB_RECURSE ALL_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
)

add_library(Common STATIC ${ALL_SOURCES})

target_link_libraries(Common PUBLIC glfw)
target_link_libraries(Common PUBLIC assimp)

target_include_directories(Common PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(Common PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/imgui)

find_package(OpenGL REQUIRED)
target_link_libraries(Common PUBLIC OpenGL::GL)
