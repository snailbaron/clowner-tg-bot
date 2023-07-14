cmake_minimum_required(VERSION 3.0)

project(Clowner)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(deps)

add_executable(clowner
    main.cpp
)
target_link_libraries(clowner PRIVATE TgBot)
