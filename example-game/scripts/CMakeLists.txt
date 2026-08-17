cmake_minimum_required(VERSION 3.20)

project(ScriptsProject LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(ASSIMP_BUILD_IFC_IMPORTER OFF CACHE BOOL "Disable IFC" FORCE)
set(ASSIMP_BUILD_STEP_IMPORTER OFF CACHE BOOL "Disable STEP" FORCE)
set(ASSIMP_BUILD_GLTF_IMPORTER OFF CACHE BOOL "Disable GLTF" FORCE)
set(ASSIMP_BUILD_COLLADA_IMPORTER OFF CACHE BOOL "Disable Collada" FORCE)
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "Disable tests" FORCE)
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "Disable tools" FORCE)
set(ASSIMP_INSTALL OFF CACHE BOOL "Disable install targets" FORCE)
set(HUNTER_ENABLED OFF CACHE BOOL "Disable Hunter package manager" FORCE)

include(FetchContent)

FetchContent_Declare(
    Engine SOURCE_DIR "C:/Users/jorda/source/repos/JordanJRead/OpenGLRenderer" SOURCE_SUBDIR "Common"
)

FetchContent_MakeAvailable(Engine)

file(
    GLOB_RECURSE ALL_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp"
    "factories.cpp"
    "imguicontext.cpp"
)

add_library(Scripts MODULE ${ALL_SOURCES})

if (MSVC)
    target_compile_options(Scripts PRIVATE "/Z7")
    set_target_properties(Scripts PROPERTIES LINK_FLAGS "/DEBUG:NONE")
endif()

target_include_directories(Scripts PRIVATE 
    "C:/Users/jorda/source/repos/JordanJRead/OpenGLRenderer/Common/include"
)

set_target_properties(Scripts PROPERTIES DEBUG_POSTFIX "")

target_link_libraries(Scripts PRIVATE Common)