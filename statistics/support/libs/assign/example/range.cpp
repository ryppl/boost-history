//////////////////////////////////////////////////////////////////////////////
// test::chain.cpp                                                          //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////
#include <ostream>
#include <iterator>
#include <vector>
#include <boost/array.hpp>
#include <boost/typeof/typeof.hpp>
#include <boost/next_prior.hpp>
#include <boost/range/algorithm/copy.hpp>
#include <boost/assign/auto_size/ref_list_of.hpp>
#include <boost/assign/auto_size/range/basic_chain.hpp>
#include <boost/assign/auto_size/range/chain_l.hpp>
#include <boost/assign/auto_size/range/chain_r.hpp>
#include <boost/assign/auto_size/range/convert_range.hpp>

#include <libs/assign/example/range.h>

void example_range(std::ostream& os)
{
 
    using namespace boost::assign;
    using namespace boost::assign::detail;
	os << "-> example_range: " << std::endl;
    typedef int val_;
	const val_ a1 = 1, b1 = 2, c1 = 3, d1 = 4, e1 = 5, f1 = 6, g1 = 7, h1 = 8, i1 = 9;
	      val_ a = a1, b = b1, c = c1, d = d1, e = e1, f = f1, g = g1, h = h1, i = i1;
    typedef boost::array<val_,2> ar_;
    ar_ ar1; ar1.assign( -1 );
    ar_ ar2; ar2.assign( 0 );
    ar_ ar3; ar3.assign( 1 );
    ar_ ar4; ar4.assign( 2 );
    ar_ ar5; ar5.assign( 3 );
    ar_ ar6; ar6.assign( 6 );

    typedef boost::assign::detail::auto_size::n_th_expr_copy<val_,2>::type inp_;

    typedef detail::result_of::convert_range<ar_,val_>::type range_;

    typedef boost::range_reference<range_>::type ref_;
//    typedef boost::assign::detail::reference_traits::convert_to<ref_>::type to_;

    BOOST_MPL_ASSERT(( boost::is_same<ref_,val_&> ));    
    
{   

    BOOST_AUTO(tmp1, ref_list_of(a)(b) );
    BOOST_AUTO(tmp2, ref_list_of(c)(d) );
    BOOST_AUTO(tmp3, ref_list_of(e)(f) );

    typedef std::vector<val_> vec_;

    os << " tmp1 && tmp2 && tmp3 = (";
    boost::copy(
        tmp1 && tmp2 && tmp3,
        std::ostream_iterator<val_>(os," ")
    ); 

    tmp1 && ar5 && ref_list_of(e)(f);

    os <<  ") becomes (";
    boost::copy(
        ar1 && ar2 && ar3 ,
        boost::begin( tmp1 && ar5 && tmp3 )
    );  
    boost::copy(
        tmp1 && ar5 && tmp3,
        std::ostream_iterator<val_>(os," ")
    );
    os << ") should equal (";
    boost::copy(
        ar1 && ar2 && ar3,
        std::ostream_iterator<val_>(os," ")
    ); os << ')' << std::endl;


}
	os << "<- " << std::endl;
    
}
