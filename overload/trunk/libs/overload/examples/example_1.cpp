/*=============================================================================
    Boost Overload    
    Example 1 - Basic usage
==============================================================================*/


#include <boost/detail/lightweight_test.hpp>

#include <boost/overload.hpp>


using boost::overload;


int int_sum(int x, int y)
{
    return x + y;
}

float float_inc(float x )
{
    return x + 1.0f;
}

int main()
{
    overload<int (int, int ), float (float )> f;

    f.set(&int_sum);     // here automatic signature
    f.set(&float_inc);   // deduction occurs

    int r1 = f(1, 2);    // calls int_sum
    float r2 = f(3.0f);  // calls float_inc

    BOOST_ASSERT(r1 == 3);
    BOOST_ASSERT(r2 == 4.0f);

    return boost::report_errors();
}

