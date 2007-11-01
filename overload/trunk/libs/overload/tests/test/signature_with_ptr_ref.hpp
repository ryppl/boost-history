
#ifndef _TEST_SIGNATURE_WITH_PTR_AND_REF_HPP_
#define _TEST_SIGNATURE_WITH_PTR_AND_REF_HPP_


void test08()
{
    boost::overload<sig10_t, sig11_t, sig12_t, sig13_t> f;

    bar9 b9;
    bar11 b11;

    f.set(&foo8);
    f.set(&bar9::foo9);
    f.set(&bar9::foo10);
    f.set(&bar11::foo11);

    double d = 5;
    double* p = &d;

    BOOST_ASSERT( f(d) == 8 );
    BOOST_ASSERT( f(&b9, &p) == 9 );
    BOOST_ASSERT( f(&b9 , p) == 10 );
    BOOST_ASSERT( f(&b11, &p) == 11 );

    f.clear_all();
    BOOST_ASSERT( f.empty_all() );

    f.set<sig10_t>(&foo8);
    f.set<sig11_t>(&bar9::foo9);
    f.set<sig12_t>(&bar9::foo10);
    f.set<sig13_t>(&bar11::foo11);

    BOOST_ASSERT( f(d) == 8 );
    BOOST_ASSERT( f(&b9, &p) == 9 );
    BOOST_ASSERT( f(&b9 , p) == 10 );
    BOOST_ASSERT( f(&b11, &p) == 11 );

}



#endif // _TEST_SIGNATURE_WITH_PTR_AND_REF_HPP_

