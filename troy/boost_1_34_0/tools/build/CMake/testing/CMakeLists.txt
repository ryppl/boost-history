#   libraries arranged like this:
#        
#          A
#        /   \
#       B     C
#        \   /
#          D       E
#
#   for checking propagation of usage requirements, dependency calculation, etc.
# 
add_subdirectory(a)
add_subdirectory(b)
add_subdirectory(c)
add_subdirectory(d)
add_subdirectory(e)