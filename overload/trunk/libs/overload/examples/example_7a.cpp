/*=============================================================================
    Boost Overload    
    Example 7a - Setting a free function overload as callable target
==============================================================================*/


#include <string>

#include <boost/detail/lightweight_test.hpp>

#include <boost/overload.hpp>


using boost::overload;


int foo(int )
{
    return 1;
}

int foo(std::string )
{
    return 2;
}

int main()
{
   overload<int (int ), int (std::string )> f;

   int (*foo1) (int ) = &foo;
   int (*foo2) (std::string ) = &foo;

   f.set(foo1);
   f.set(foo2);

   BOOST_ASSERT( f(0) == 1 );
   BOOST_ASSERT( f("hi") == 2 );

   return boost::report_errors();
}

