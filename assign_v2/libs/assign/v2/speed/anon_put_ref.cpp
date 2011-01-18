//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <vector>
#include <deque>
#include <string>

#include <boost/preprocessor/control/if.hpp>
#include <boost/preprocessor/control/expr_if.hpp>
#include <boost/preprocessor/comparison/less_equal.hpp>
#include <boost/preprocessor/comparison/greater.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/seq/first_n.hpp>
#include <boost/preprocessor/repetition/enum_params_with_a_default.hpp>
#include <boost/preprocessor/seq/for_each.hpp>

#include <boost/format.hpp>
#include <boost/progress.hpp>
#include <boost/typeof/typeof.hpp>

#include <boost/assign/list_of.hpp>
#include <boost/assign/list_inserter.hpp>
#define BOOST_ASSIGN_V2_LIMIT_CSV_ARITY 128
#include <boost/assign/v2.hpp>

#include <libs/assign/v2/speed/tools.h>
#include <libs/assign/v2/speed/anon_put_ref.h>

namespace speed_assign_v2{
namespace xxx_anon_put_ref{

void run(std::ostream& os)
{

    using namespace boost::assign;
    using namespace v2;

    os << "-> Runtime efficiency " << std::endl;

    typedef boost::timer timer_;
    typedef std::string str_;
    typedef boost::format f_;

// [---- Warning : don't override these markups as they are used by the doc

    //[types
    typedef std::vector<int> elem_; // T
    // This STL container is of the same type as that internally used by the
    // anon container of values, so we have a common basis for comparing
    // their exec time.
    typedef std::deque< elem_ > cont_;
    //]
    //[sim_param
    // t is a random size element, with max size :
    const int elem_size = 100;
    // Number of loops over which exec time is summed
    const int n_loops = 1000 * 1000;
    //]

// ----]

// The number of values is n :
// 	fun(t[0], ....,t[n-1])
// 	fun( t[0] )...( t[n-1] )
// The mapping N -> n is
// [1-> 1, 2-> 2, 3->4, 4->8, 5->16, 6->32, 7->64, 8->128]
// Warning : limit_N > 6 may cause a long compilation time
#define limit_N 0
#if BOOST_PP_GREATER(limit_N, 8)
#error
#endif

    elem_ BOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT( 128, a, rand_vec(elem_size) );

	{
    	str_ str = "This test times the filling of a std::deque<T>";
        os << str << std::endl;
    }
	{
    	f_ f("where T = std::vector<int>, and has max size %1%");
        f % elem_size;
        os << f.str() << std::endl;
    }
    {
		f_ f("Number of loops over which execution time is summed : %1% ");
    	f % n_loops;
        os << f.str() << std::endl;
    }

#define SEQ (a0)(a1)(a2)(a3)(a4)(a5)(a6)(a7)(a8)(a9)(a10)(a11)(a12)(a13)(a14)\
(a15)(a16)(a17)(a18)(a19)(a20)(a21)(a22)(a23)(a24)(a25)(a26)(a27)(a28)(a29)\
(a30)(a31)(a32)(a33)(a34)(a35)(a36)(a37)(a38)(a39)(a40)(a41)(a42)(a43)(a44)\
(a45)(a46)(a47)(a48)(a49)(a50)(a51)(a52)(a53)(a54)(a55)(a56)(a57)(a58)(a59)\
(a60)(a61)(a62)(a63)(a64)(a65)(a66)(a67)(a68)(a69)(a70)(a71)(a72)(a73)(a74)\
(a75)(a76)(a77)(a78)(a79)(a80)(a81)(a82)(a83)(a84)(a85)(a86)(a87)(a88)(a89)\
(a90)(a91)(a92)(a93)(a94)(a95)(a96)(a97)(a98)(a99)(a100)(a101)(a102)(a103)\
(a104)(a105)(a106)(a107)(a108)(a109)(a110)(a111)(a112)(a113)(a114)(a115)(a116)\
(a117)(a118)(a119)(a120)(a121)(a122)(a123)(a124)(a125)(a126)(a127)\
/**/
#define MACRO1(fun, N, param)\
    {\
        timer_ timer;\
        for(int i = 0; i < n_loops; ++i)\
        {\
            BOOST_AUTO(\
                rng,  \
                (\
					fun BOOST_PP_IF(\
                    	param,\
                    	BOOST_PP_SEQ_FIRST_N(N, SEQ),\
                        ( BOOST_PP_ENUM_PARAMS(N, a) )\
                    )\
                )\
            );\
            int sz = (int)rng.size();\
            if(sz != N)\
                os << "ERROR\n";\
        }\
        elapsed = timer.elapsed();\
    }\
/**/
#define MACRO_push_back(z, data, elem ) cont.push_back( elem );
#define MACRO2(N, param1, param2, param3, param4, param5, param6)\
    {\
        timer_ timer;\
        for(int i = 0; i < n_loops; ++i)\
        {\
        	cont.clear();\
            BOOST_PP_EXPR_IF(\
            	param1,\
                BOOST_PP_SEQ_FOR_EACH(\
                	MACRO_push_back,\
                    ~,\
                    BOOST_PP_SEQ_FIRST_N(N, SEQ)\
                );\
            )\
            BOOST_PP_EXPR_IF(\
            	param2, \
                put(cont) BOOST_PP_SEQ_FIRST_N(N, SEQ);\
            )\
            BOOST_PP_EXPR_IF(\
            	param3, \
                csv_put(cont)( BOOST_PP_ENUM_PARAMS(N, a) );\
            )\
            BOOST_PP_EXPR_IF(\
            	param4, \
                using namespace adaptor;\
                cont | _put BOOST_PP_SEQ_FIRST_N(N, SEQ);\
            )\
            BOOST_PP_EXPR_IF(\
            	param5, \
                using namespace adaptor;\
                cont | _csv_put( BOOST_PP_ENUM_PARAMS(N, a) );\
            )\
            BOOST_PP_EXPR_IF(\
            	param6, \
                push_back(cont) BOOST_PP_SEQ_FIRST_N(N, SEQ);\
            )\
            int sz = (int)cont.size();\
            if(sz != N)\
                os << "ERROR\n";\
        }\
		elapsed = timer.elapsed();\
    }\
/**/

    cont_ cont;

    str_ str_n = "[";
    str_ str_t = "[";

    for(int i = 0; i < limit_N ; i++)
    {
        f_ f( "%1%" ); f % ( i + 1 );
        str_n += "%" + f.str() + "% ";
        str_t += "%" + f.str() + "% " + "s ";
    }
    str_n += "]";
    str_t += "]";

    //os << str_n << std::endl;
    //os << str_t << std::endl;

    str_ str_stl_push_back = "vec.push_back( t0 ); ... vec.push_back( tN-1 );";

    str_ str_v1_cref_list_of = "cref_list_of<N>( t0 )...( tN-1 )";
    str_ str_v1_list_of = "list_of( t0 )...( tN-1 )";
    str_ str_v1_push_back = "push_back( vec )( t0 )...( tN-1 )";

    str_ str_v2_csv_anon = "csv_anon(t0, ...,tN-1)";
    str_ str_v2_anon = "anon<T>( _nil )( t0 )...( tN-1 )";
    str_ str_v2_ref_csv_anon = "ref::assign_copy::csv_anon(t0, ...,tN-1)";
    str_ str_v2_ref_anon = "ref::assign_copy::anon( t0 )...( tN-1 )";
    str_ str_v2_csv_put = "csv_put( vec )(t0, ...,tN-1)";
    str_ str_v2_put = "put( vec )( t0 )...( tN-1 )";
    str_ str_v2_adaptor_csv_put = "vec | _csv_put(t0, ...,tN-1)";
    str_ str_v2_adaptor_put = "vec | _put( t0 )...( tN-1 )";

	f_ f_n( str_( "n = " ) + str_n );

    f_ f_stl_push_back( str_stl_push_back + str_t );

    f_ f_v1_list_of( str_v1_list_of + str_t );
    f_ f_v1_cref_list_of( str_v1_cref_list_of + str_t );
    f_ f_v1_push_back( str_v1_push_back + str_t );

    f_ f_v2_csv_anon( str_v2_csv_anon + str_t );
    f_ f_v2_anon( str_v2_anon + str_t ) ;
    f_ f_v2_ref_csv_anon( str_v2_ref_csv_anon + str_t );
    f_ f_v2_ref_anon( str_v2_ref_anon + str_t );
    f_ f_v2_csv_put( str_v2_csv_put + str_t );
    f_ f_v2_put( str_v2_put + str_t );
    f_ f_v2_adaptor_csv_put( str_v2_adaptor_csv_put + str_t );
    f_ f_v2_adaptor_put( str_v2_adaptor_put + str_t );

// MACRO2
// param1 : stl_push_back
// param2 : v2_put
// param3 : v2_csv_put
// param4 : v2_adaptor_put
// param5 : v2_adaptor_csv_put
// param6 : v1_push_back

#define MACRO(N)\
    os << "N = " << N << " . . . ";\
    {\
    	MACRO2( N, 1, 0, 0, 0, 0, 0 )\
        f_stl_push_back % elapsed;\
    	MACRO2( N, 0, 1, 0, 0, 0, 0 )\
        f_v2_put % elapsed;\
    	MACRO2( N, 0, 0, 1, 0, 0, 0 )\
        f_v2_csv_put % elapsed;\
        MACRO2( N, 0, 0, 0, 1, 0, 0 )\
        f_v2_adaptor_put % elapsed;\
    	MACRO2( N, 0, 0, 0, 0, 1, 0 )\
        f_v2_adaptor_csv_put % elapsed;\
    	MACRO2( N, 0, 0, 0, 0, 0, 1 )\
        f_v1_push_back % elapsed;\
		\
    	MACRO1( boost::assign::list_of, N, 1 )\
        f_v1_list_of % elapsed;\
        \
    	MACRO1( boost::assign::cref_list_of<N>, N, 1 )\
        f_v1_cref_list_of % elapsed;\
        \
    	MACRO1( boost::assign::v2::csv_anon, N, 0 )\
        f_v2_csv_anon % elapsed;\
        \
    	MACRO1( boost::assign::v2::anon<elem_>( _nil ), N, 1 )\
        f_v2_anon % elapsed;\
        \
    	MACRO1( boost::assign::v2::ref::assign_copy::csv_anon, N, 0 )\
        f_v2_ref_csv_anon % elapsed;\
		\
    	MACRO1( boost::assign::v2::ref::assign_copy::anon, N, 1 )\
        f_v2_ref_anon % elapsed;\
		\
        f_n % N;\
    }\
/**/

    double elapsed = 0; 
    if(elapsed){ 
        //nothing
    }
// -----------------------------------N------------------n----------
// Dont uncomment. Modify instead limit_N
BOOST_PP_EXPR_IF( BOOST_PP_LESS_EQUAL(1, limit_N), MACRO(1) )
BOOST_PP_EXPR_IF( BOOST_PP_LESS_EQUAL(2, limit_N), MACRO(2) )
BOOST_PP_EXPR_IF( BOOST_PP_LESS_EQUAL(3, limit_N), MACRO(4) )
BOOST_PP_EXPR_IF( BOOST_PP_LESS_EQUAL(4, limit_N), MACRO(8) )
BOOST_PP_EXPR_IF( BOOST_PP_LESS_EQUAL(5, limit_N), MACRO(16) )
BOOST_PP_EXPR_IF( BOOST_PP_LESS_EQUAL(6, limit_N), MACRO(32) )
BOOST_PP_EXPR_IF( BOOST_PP_LESS_EQUAL(7, limit_N), MACRO(64) )
BOOST_PP_EXPR_IF( BOOST_PP_LESS_EQUAL(8, limit_N), MACRO(128) )

#undef limit_n
#undef SEQ
#undef MACRO_push_back
#undef MACRO1
#undef MACRO2
#undef MACRO
#undef BOOST_ASSIGN_V2_LIMIT_CSV_ARITY

	os << std::endl;
	os << f_n.str() << std::endl;
	os << f_stl_push_back.str() << std::endl;

    os << f_v1_list_of.str() << std::endl;
    os << f_v1_cref_list_of.str() << std::endl;
    os << f_v1_push_back.str() << std::endl;

    os << f_v2_csv_anon.str() << std::endl;
    os << f_v2_anon.str() << std::endl;
    os << f_v2_ref_csv_anon.str() << std::endl;
    os << f_v2_ref_anon.str() << std::endl;
    os << f_v2_csv_put.str() << std::endl;
    os << f_v2_put.str() << std::endl;
    os << f_v2_adaptor_csv_put.str() << std::endl;
    os << f_v2_adaptor_put.str() << std::endl;

    os << "<- " << std::endl;

}

}// xxx_anon_put_ref
}// speed_assign_v2
