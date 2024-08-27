cmake_minimum_required(VERSION 3.10)

project(CANParser VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(CANParser src/main.cpp)

# include header files
include_directories(~/dbcppp/include)

# linking libraries
target_link_libraries(CANParser ~/dbcppp/build/libdbcppp.so)


