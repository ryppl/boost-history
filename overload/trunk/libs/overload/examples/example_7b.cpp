/*=============================================================================
    Boost Overload    

    Example 7b - Setting a free function overload as callable target
==============================================================================*/


#include <string>

#include <boost/detail/lightweight_test.hpp>

#include "../../../boost/overload/overload.hpp"


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

   // disambiguation through the signature based sintax
   f.set<int (int )>(&foo);
   f.set<int (std::string )>(&foo);

   BOOST_ASSERT( f(0) == 1 );
   BOOST_ASSERT( f("hi") == 2 );

   return boost::report_errors();
}

