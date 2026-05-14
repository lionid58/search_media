cmake_minimum_required(VERSION 3.16)
project(WebTop LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_BUILD_TYPE Debug)

# Включаем FetchContent
include(FetchContent)

# Скачиваем cpp-httplib
FetchContent_Declare(
    cpp-httplib
    GIT_REPOSITORY https://github.com/yhirose/cpp-httplib.git
    GIT_TAG v0.15.3
    GIT_SHALLOW TRUE  # быстрее скачивать
)
FetchContent_MakeAvailable(cpp-httplib)

# Найти nlohmann_json
find_package(nlohmann_json REQUIRED)

add_executable(media_parser 
    src/main.cpp
    src/file_parser.cpp
)

target_include_directories(media_parser PRIVATE 
    ${cpp-httplib_SOURCE_DIR}
)

target_link_libraries(media_parser PRIVATE 
    nlohmann_json::nlohmann_json
)