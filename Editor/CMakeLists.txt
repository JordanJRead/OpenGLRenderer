file(
    GLOB_RECURSE ALL_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
)

add_executable(Editor ${ALL_SOURCES})

# Include
target_include_directories(Editor PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(Editor PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../Common/include)

# Libraries
target_link_libraries(Editor PRIVATE Common)

find_package(OpenGL REQUIRED)
target_link_libraries(Common PRIVATE OpenGL::GL)

# Post build
add_custom_command(
    TARGET Editor POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_CURRENT_SOURCE_DIR}/ExecutableDirContents/"
    "$<TARGET_FILE_DIR:Editor>"
)