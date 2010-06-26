The tests in libs/composite_storage/sandbox/pack were compiled and run
with:
 
ftp://gcc.gnu.org/pub/gcc/snapshots/4.5-20100408/gcc-g++-4.5-20100408.tar.bz2

An earlier version:

ftp://gcc.gnu.org/pub/gcc/snapshots/4.4-20090630/gcc-4.4-20090630.tar.bz2

failed to compile one test and gave error message:

one_of_multiple_dispatch.test.cpp:371:   instantiated from here
../../../../boost/composite_storage/pack/multiple_dispatch/replace_source_with_target_ptr.hpp:84:
error: incompatible types in assignment of '<brace-enclosed
initializer list>' to 'void* [2]'

