//////////////////////////////////////////////////////////////////////////////
// test::chain.cpp                                                          //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////
#include <ostream>
#include <iterator>
#include <vector>
#include <boost/concept/assert.hpp>
#include <boost/array.hpp>
#include <boost/typeof/typeof.hpp>
#include <boost/next_prior.hpp>
#include <boost/range/algorithm/copy.hpp>
#include <boost/assign/auto_size/ref_list_of.hpp>
#include <boost/assign/auto_size/detail/expr.hpp>
#include <boost/assign/auto_size/reference_wrapper/conversion_traits.hpp>
//#include <boost/assign/auto_size/chain/chain.hpp>
//#include <boost/assign/auto_size/comparison_op/crtp.hpp>

#include <libs/assign/example/chain.h>

void example_chain(std::ostream& os)
{

    //boost::assign::detail::chain_mpl_check::compound();
 
    using namespace boost::assign;
    using namespace boost::assign::detail;
	os << "-> example_chain: " << std::endl;
    typedef int val_;
	val_ a = 1, b = 2, c = 3, d = 4, e = 5, f = 6, g = 7, h = 8, i = 9 ;
    typedef boost::array<val_,4> ar_;
    typedef std::vector<val_> vec_;
    ar_ ar1; ar1.assign( -1 );
    ar_ ar2; ar2.assign( 0 );
    ar_ ar3; ar3.assign( 1 );
    ar_ ar4; ar4.assign( 2 );
    ar_ ar5; ar5.assign( 3 );
    typedef detail::auto_size::n_th_expr_copy<val_,4>::type res4_;
	res4_ tmp1 = ref_list_of(a)(b)(c)(d); 
	BOOST_AUTO(tmp2,ref_list_of(e)(f)(g)(h)); 

/*
    boost::copy(
        chain_r(tmp2)(ar4)(ar5),
        std::ostream_iterator<val_>(os," ")
    ); os << " --- becomes ---> " << std::endl;

    boost::copy(
        chain_r(tmp1)(ar2)(ar3),
        boost::begin(tmp)
    ); 

    boost::copy(
        chain_r(tmp2)(ar4)(ar5),
        std::ostream_iterator<val_>(os," ")
    ); os << " --- that should equal ---> "<< std::endl;

    boost::copy(
        chain_r(ref_list_of(a)(b)(c)(d))(ar2)(ar3),
        std::ostream_iterator<val_>(os," ")
    ); os << " --- tier segments in reverse order ---> " << std::endl;

    boost::copy(
        chain_r(ar3)(ar2)(ref_list_of(a)(b)(c)(d)),
        std::ostream_iterator<val_>(os," ")
    ); // this is a case where without _r, there would be compile error

   // Why chain is required :
   //invalid initialization of reference of type 'boost::assign::detail::assign_reference_copy<example_chain::val_>&' 
   // from expression of type 'int'
   //boost::copy(
   //     boost::chain(tmp1,ar2),
   //     boost::begin(
   //         boost::chain(tmp2,ar4)
   //     )
   //);
*/
	os << "<- " << std::endl;
    
}
