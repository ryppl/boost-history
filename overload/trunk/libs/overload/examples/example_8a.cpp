/*=============================================================================
    Boost Overload    
    Example 8a - Setting a member function overload as callable target
==============================================================================*/


#include <string>

#include <boost/detail/lightweight_test.hpp>

#include <boost/overload.hpp>


using boost::overload;


struct bar
{
   int foo(int )
   {
       return 1;
   }

   int foo(std::string )
   {
       return 2;
   }
};

int main()
{
   overload<int (bar*, int ), int (bar*, std::string )> f;

   bar b;

   int (bar::*foo1) (int ) = &bar::foo;
   int (bar::*foo2) (std::string ) = &bar::foo;

   f.set(foo1);
   f.set(foo2);

   BOOST_ASSERT( f(&b, 0) == 1 );
   BOOST_ASSERT( f(&b, "hi") == 2 );

   return boost::report_errors();
}

