/*=============================================================================
    Boost Overload    
    Example 5 - Using the signature based syntax 
                with multi-signature function objects
==============================================================================*/


#include <string>

#include <boost/detail/lightweight_test.hpp>

#include "../../../boost/overload/overload.hpp"


using boost::overload;


struct bar
{
    template<typename T>
    T operator()(T x)
    {
        return x;
    }
};

int main()
{
    overload<int (int ), std::string (std::string )> f;

    bar foo;

    f.set<int (int )>(foo);     // we are using the signature syntax that sets foo
                                // as callable target for the signature int (int ) only
                       
    int    r1 = f(1);           // calls int foo(int ) template instantiation

    /* Warning !!
       This call produces a comiler error because there is no callable target
       tied to the "std::string (std::string )" signature
       std:string r2 = f( std::string("hi") );                      
    */

    BOOST_ASSERT(r1 == 1);

    return boost::report_errors();
}

