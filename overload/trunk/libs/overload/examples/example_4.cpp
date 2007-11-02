/*=============================================================================
    Boost Overload    
    Example 4 - Template function object
==============================================================================*/


#include <string>

#include <boost/detail/lightweight_test.hpp>

#include <boost/overload.hpp>


using boost::overload;


char foo2(std::string )
{
    return 'x';
}

struct bar
{
    template<typename T>
    T operator()(T x)
    {
        return x + 1;
    }
};

int main()
{
    overload<char (std::string ), int (int ), double (double )> f;

    bar foo;

    f.set(foo);     // sets foo as callable target for both the signature int (int )
                    // and the signature double (double )

    f.set(&foo2);

    char   r1 = f("hi");
    int    r2 = f(1);          // calls int foo(int ) template instantiation
    double r3 = f(2.0);        // calls double foo(double ) template instantiation

    BOOST_ASSERT(r1 == 'x');
    BOOST_ASSERT(r2 == 2);
    BOOST_ASSERT(r3 == 3.0);

    return boost::report_errors();
}

