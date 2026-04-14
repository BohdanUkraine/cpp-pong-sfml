cmake_minimum_required(VERSION 3.15)
project(pong)

if(WIN32)
    set(SFML_DIR "${CMAKE_CURRENT_SOURCE_DIR}/external/lib/cmake/SFML")
endif()

find_package(SFML 3.0 COMPONENTS Graphics Window System Audio REQUIRED)

file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "headers/*.h")

add_executable(pong ${SOURCES} ${HEADERS})

target_include_directories(pong PRIVATE headers)
if(WIN32)
    target_include_directories(pong PRIVATE external/include)
endif()

target_link_libraries(pong PRIVATE SFML::Graphics SFML::Window SFML::System SFML::Audio)

if(WIN32)
    add_custom_command(TARGET pong POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/external/bin"
        "$<TARGET_FILE_DIR:pong>")
endif()

add_custom_command(TARGET pong POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_CURRENT_SOURCE_DIR}/assets"
    "$<TARGET_FILE_DIR:pong>/assets")


target_compile_features(pong PRIVATE cxx_std_17)


target_compile_definitions(pong PRIVATE WINDOW_HEIGHT=600)
target_compile_definitions(pong PRIVATE WINDOW_WIDTH=800)
