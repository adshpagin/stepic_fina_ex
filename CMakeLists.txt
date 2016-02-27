cmake_minimum_required(VERSION 2.8)
project(final)

set(SOURCES main.cxx tcp.cxx http.cxx log.cxx logic.cxx)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g -O0 -Wall")
set(LIBRARIES uv)
link_directories(/usr/local/lib)

add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} ${LIBRARIES})
