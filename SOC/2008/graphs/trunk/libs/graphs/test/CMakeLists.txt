
include_directories(${Graphs_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS})

# add_exec(vertex_store vertex_store.cpp)
# add_exec(edge_store edge_store.cpp)

add_exec(undirected undirected.cpp)
