
#ifndef _TEST_SIGNATURE_DEDUCTION_HPP_
#define _TEST_SIGNATURE_DEDUCTION_HPP_


void test05() 
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

    bar1 b1; bar2 b2;

    overload_type f(f4, f2);
    f.set(f0, f5, f1, f3);

    f( "Hello",  ", ",  "world !" );

    int i       = f(&b1, 'x');
    double d1   = f(&b2, 123, 'x');
    char c      = f("hello");
    int j       = f('x');
    double d2   = f(123, 'x');

    BOOST_ASSERT(i == 123);
    BOOST_ASSERT(d1 > 123.455 && d1 < 123.457);
    BOOST_ASSERT(c == 'x');
    BOOST_ASSERT(j == 123);
    BOOST_ASSERT(d2 > 123.455 && d2 < 123.457);


    //////////////////////////////////////////////////////////////////////////
    // multi-signature function objects

    // MSVC 7.1 doesn't succeed in deducing the signature of a multi-signature 
    // function object
#if !BOOST_WORKAROUND(BOOST_MSVC, < 1400)  
 
    f6_t f6;                        // overloaded function object
    f7_t f7;                        // polymorfic function object

    f.set(f7, f6);

    double d3   = f(2.5f);
    double d4   = f(3.0f, 4.0f);
    int k       = f(1);
    double d5   = f(2.5);

    BOOST_ASSERT(d3 == 3.5);
    BOOST_ASSERT(d4 == 7.0);
    BOOST_ASSERT(k == 2);
    BOOST_ASSERT(d5 == 3.5);



    boost::overload<int (int, int), int (int, std::string )> g;
    f14_t f14;
    g.set(f14);
    BOOST_ASSERT( g(1,1) == sizeof(int) );
    BOOST_ASSERT( g(1, std::string("hi")) == sizeof(std::string) );

#endif

}


#endif // _TEST_SIGNATURE_DEDUCTION_HPP_

