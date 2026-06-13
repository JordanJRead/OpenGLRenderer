# DLLs
add_library(glfwLib SHARED IMPORTED)

set_target_properties(glfwLib PROPERTIES IMPORTED_IMPLIB ${CMAKE_CURRENT_SOURCE_DIR}/lib/glfw3dll.lib)
set_target_properties(glfwLib PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/lib/glfw3.dll)

add_library(assimpLib SHARED IMPORTED)

set_target_properties(assimpLib PROPERTIES IMPORTED_IMPLIB ${CMAKE_CURRENT_SOURCE_DIR}/lib/assimp-vc143-mtd.lib)
set_target_properties(assimpLib PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/lib/assimp-vc143-mtd.dll)

file(
    GLOB_RECURSE ALL_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
)

add_library(Common STATIC ${ALL_SOURCES})

target_link_libraries(Common PRIVATE glfwLib)
target_link_libraries(Common PRIVATE assimpLib)

find_package(OpenGL REQUIRED)
target_link_libraries(Common PRIVATE OpenGL::GL)

target_include_directories(Common PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(Common PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/imgui)