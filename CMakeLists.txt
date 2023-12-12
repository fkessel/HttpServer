cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 20)
project(HttpServer VERSION 1.0)
enable_testing()

add_executable(HttpServer 
    main.cpp
)
find_package(Threads)
target_link_libraries(HttpServer Threads::Threads)

# add_executable(Zukunft
#     src/zukunft.cpp
# )
# target_link_libraries(Zukunft Threads::Threads)

# add_executable(AlterEgo
#     src/ego.cpp
# )
# target_link_libraries(AlterEgo Threads::Threads)

add_subdirectory(tests)