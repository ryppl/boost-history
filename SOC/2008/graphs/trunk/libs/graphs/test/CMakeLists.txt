
include_directories(${Graphs_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS})

add_exec(hello hello.cpp)
add_exec(vertex_store vertex_store.cpp)
