cmake_minimum_required(VERSION 3.10)

project(scalable_tcp_server)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# include directory
include_directories(include)

# collect all source files automatically
file(GLOB_RECURSE SOURCES
    "${CMAKE_SOURCE_DIR}/src/*.cpp"
)

# build executable (output goes to build/)
add_executable(server ${SOURCES})

find_package(Boost REQUIRED COMPONENTS json)
target_link_libraries(server PRIVATE Boost::json)
