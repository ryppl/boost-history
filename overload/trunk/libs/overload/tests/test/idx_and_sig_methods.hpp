
#ifndef TEST_IDX_AND_SIG_METHODS_HPP
#define TEST_IDX_AND_SIG_METHODS_HPP

void test06()
{
    typedef boost::overload<sig0_t, sig1_t, sig2_t, sig3_t, sig4_t, 
                            sig5_t, sig6_t, sig7_t, sig8_t, sig9_t> 
            overload_type;

    f0_t f0 = &foo0;                // function pointer
    f1_t f1 = &bar1::foo1;          // member function pointer
    f2_t f2 = &bar2::foo2;          // member function pointer (with const)
    f3_t f3 = &foo3;                // boost::function
    f4_t f4;                        // function object
    f5_t f5;                        // function object (with const)
    f6_t f6;                        // overloaded function object
    f7_t f7;                        // polymorfic function object

    bar1 b1; bar2 b2;

    overload_type f;

    f.set<0>(f0);
    f.set<sig1_t>(f1);
    f.set<5>(f5).set<3>(f3).set<sig4_t>(f4).set<2>(f2);
    f.set<sig6_t>(f6).set<7>(f6);
    f.set<sig9_t>(f7).set<sig8_t>(f7);

    f( "Hi",  ", ",  "people !" );

    int i       = f(&b1, 'x');
    double d1   = f(&b2, 123, 'x');
    char c      = f("hello");
    int j       = f('x');
    double d2   = f(123, 'x');
    double d3   = f(2.5f);
    double d4   = f(3.0f, 4.0f);
    int k       = f(1);
    double d5   = f(2.5);

    BOOST_ASSERT(i == 123);
    BOOST_ASSERT(d1 > 123.455 && d1 < 123.457);
    BOOST_ASSERT(c == 'x');
    BOOST_ASSERT(j == 123);
    BOOST_ASSERT(d2 > 123.455 && d2 < 123.457);
    BOOST_ASSERT(d3 == 3.5);
    BOOST_ASSERT(d4 == 7.0);
    BOOST_ASSERT(k == 2);
    BOOST_ASSERT(d5 == 3.5);


    f.clear<sig0_t>();                      // same as f.clear<0>()
    BOOST_ASSERT( f.empty<sig0_t>() );      // same as f.empty<0>()

    boost::function<sig0_t> g(f0);
    f.swap_function(g);
    f("I'm ", "back ", "again !");
    BOOST_ASSERT( g.empty() );

    g = f.get<sig0_t>();                    // same as f.get<0>()
    g("That's ", "all ", "folks !");

}



#endif // TEST_IDX_AND_SIG_METHODS_HPP

