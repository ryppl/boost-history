
#ifndef _TEST_FREE_AND_MEMBER_OVERLOADED_FUNCTIONS_HPP_
#define _TEST_FREE_AND_MEMBER_OVERLOADED_FUNCTIONS_HPP_


void test07()
{
    typedef 
        boost::overload<inc_int_sig_t, inc_float_sig_t, 
                        foo_int_sig_t, foo_float_sig_t>
        overload_type;

    overload_type f;

    multi multi1;

    f.set<inc_int_sig_t>(&inc);                         // uses "int inc(int )"
    f.set<inc_float_sig_t>(&inc);                       // uses "float inc(float )"
    f.set<foo_int_sig_t, ovld::non_const>(&multi::foo); // uses "int foo(int )"
    f.set<foo_float_sig_t>(&multi::foo);                // uses "float foo(std::string )"
 
    BOOST_ASSERT( f(1) == 2 );
    BOOST_ASSERT( f(2.0f) == 3.0f );
    BOOST_ASSERT( f(&multi1, 1) == 1 );
    BOOST_ASSERT( f(&multi1, "hi") == 3.0f );

    // uses "int foo(int ) const"
    f.set<foo_int_sig_t, ovld::const_>(&multi::foo);
    BOOST_ASSERT( f(&multi1, 1) == 2 );



    boost::overload<foo_int_const_sig_t, foo_float_sig_t> g;

    // uses "int foo(int ) const" because it's the only valid choice
    g.set<foo_int_const_sig_t>(&multi::foo);
    BOOST_ASSERT( g(&multi1, 1) == 2 );

    // you can utilize the explicit syntax, too:
    // g.set<foo_int_const_sig_t, ovld::const_>(&multi1::foo);



    typedef double non_const_sig2_t (bar2*, int, char );
    boost::overload<sig1_t, non_const_sig2_t> qui;
    bar1 b1;
    bar2 b2;
    qui.set<sig1_t>(&bar1::foo1);
    qui.set<non_const_sig2_t>(&bar2::foo2);
    BOOST_ASSERT( qui(&b1, 'x') == 123 );
    BOOST_ASSERT( qui(&b2, 1, 'x') == 123.456 );
    qui.clear<non_const_sig2_t>();
    qui.set<non_const_sig2_t, ovld::const_>(&bar2::foo2);
    BOOST_ASSERT( qui(&b2, 1, 'x') == 123.456 );



    boost::overload<sig1_t, sig2_t> quo;
    quo.set<sig1_t>(&bar1::foo1);
    quo.set<sig2_t>(&bar2::foo2);
    BOOST_ASSERT( quo(&b1, 'x') == 123 );
    BOOST_ASSERT( quo(&b2, 1, 'x') == 123.456 );
    quo.clear<sig2_t>();
    quo.set<sig2_t, ovld::const_>(&bar2::foo2);
    BOOST_ASSERT( quo(&b2, 1, 'x') == 123.456 );



    boost::overload<int (int, int), int (int, std::string )> qua;
    qua.set<int (int, std::string )>(&foo12);
    qua.set<int (int, int )>(&foo12);
    BOOST_ASSERT( qua(1, 1) == 12 );
	BOOST_ASSERT( qua(1, std::string("hi")) == 122 );



    boost::overload<int (bar13*, int, int), int (bar13*, int, std::string )> duck;
    bar13 b13;
    duck.set<int (bar13*, int, int)>(&bar13::foo13);
    duck.set<int (bar13*, int, std::string )>(&bar13::foo13);
    BOOST_ASSERT( duck(&b13, 1, 1) == 13 );
    BOOST_ASSERT( duck(&b13, 1, std::string("hi")) == 133 );
}


#endif // _TEST_FREE_AND_MEMBER_OVERLOADED_FUNCTIONS_HPP_

