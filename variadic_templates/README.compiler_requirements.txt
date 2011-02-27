As of Date=2011-02-27:

  There's two compilers, that I(Larry Evans) know of that can compiler
  variadic templates.  The first is gcc.  Recently (2011 Jan), clang
  has been able to compile variadic templates; however, it still has
  some problems, unrelated to variadic templates, on linux:
  
    http://lists.cs.uiuc.edu/pipermail/cfe-dev/2011-February/013533.html
    
  For g++, only later versions (>= g++4.5) will work:
  
    The tests in libs/composite_storage/sandbox/pack were compiled and
    run with:
     
    ftp://gcc.gnu.org/pub/gcc/snapshots/4.5-20100408/gcc-g++-4.5-20100408.tar.bz2
    
    An earlier version:
    
    ftp://gcc.gnu.org/pub/gcc/snapshots/4.4-20090630/gcc-4.4-20090630.tar.bz2
    
    failed to compile one test and gave error message:
    
    one_of_multiple_dispatch.test.cpp:371: instantiated from here
    ../../../../boost/composite_storage/pack/multiple_dispatch/replace_source_with_target_ptr.hpp:84:
    error: incompatible types in assignment of '<brace-enclosed
    initializer list>' to 'void* [2]'
    
  In addition, the compiler needs to be called with the:
  
   -DBOOST_MPL_CFG_NO_PREPROCESSED_HEADERS
   
  option.  This is because, otherwise, compilation of files such as:
  
    http://svn.boost.org/svn/boost/sandbox/variadic_templates/libs/composite_storage/sandbox/pack/predator_prey.cpp
    
  will result in errors, such as:
  
/home/evansl/prog_dev/boost-svn/ro/boost_1_46_0/boost/mpl/aux_/preprocessed/gcc/bind.hpp:50:22: error: expected nested-name-specifier before 'apply_wrap5'
/home/evansl/prog_dev/boost-svn/ro/boost_1_46_0/boost/mpl/aux_/preprocessed/gcc/bind.hpp:50:22: error: expected ';' at end of member declaration
/home/evansl/prog_dev/boost-svn/ro/boost_1_46_0/boost/mpl/aux_/preprocessed/gcc/bind.hpp:50:33: error: expected unqualified-id before '<' token
/home/evansl/prog_dev/boost-svn/ro/boost_1_46_0/boost/mpl/aux_/preprocessed/gcc/bind.hpp:61:22: error: expected nested-name-specifier before 'apply_wrap5'
/  
  
    
