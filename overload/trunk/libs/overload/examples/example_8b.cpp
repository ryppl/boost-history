/*=============================================================================
    Boost Overload    
    Example 8b - Setting a member function overload as callable target
==============================================================================*/


#include <string>

#include <boost/detail/lightweight_test.hpp>

#include "../../../boost/overload/overload.hpp"


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

   // disambiguation through the signature based sintax
   f.set<int (bar*, int )>(&bar::foo);
   f.set<int (bar*, std::string )>(&bar::foo);

   BOOST_ASSERT( f(&b, 0) == 1 );
   BOOST_ASSERT( f(&b, "hi") == 2 );

   return boost::report_errors();
}

