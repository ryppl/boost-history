/*=============================================================================
    Boost Overload    
    Example 3 - Overloaded function object
==============================================================================*/


#include <string>

#include <boost/detail/lightweight_test.hpp>

#include "../../../boost/overload/overload.hpp"


using boost::overload;


char foo2(std::string )
{
    return 'x';
}

struct bar
{
    double operator() (float x)
    {
        return x + 1;
    }

    double operator() (float x, float y)
    {
        return x + y;
    }
};

int main()
{
    overload<char (std::string ), double (float ), double (float, float )> f;

    bar foo;


    f.set(foo);     // sets foo as callable target for both the signature double (float )
                    // and the signature double (float, float )


    f.set(&foo2);

    char   r1 = f("hi");
    double r2 = f(1.0f);          // calls the double foo(float ) overload
    double r3 = f(2.0f, 3.0f);    // calls the double foo(float, float ) overload

    BOOST_ASSERT(r1 == 'x');
    BOOST_ASSERT(r2 == 2.0);
    BOOST_ASSERT(r3 == 5.0);

    return boost::report_errors();
}

