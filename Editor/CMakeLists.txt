file(
    GLOB_RECURSE ALL_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
)

add_executable(Editor ${ALL_SOURCES})

set_property(TARGET Editor PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:Editor>")

# Include
target_include_directories(Editor PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(Editor PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../Common/include)

# Libraries
target_link_libraries(Editor PRIVATE Common)

# Post build
add_custom_command(
    TARGET Editor POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:Editor>/assets"
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/../Common/assets/"
        "$<TARGET_FILE_DIR:Editor>/assets"
)

add_custom_command(
    TARGET Editor POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/ExecutableDirContents/"
        "$<TARGET_FILE_DIR:Editor>"
)
