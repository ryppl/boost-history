//////////////////////////////////////////////////////////////////////////////
// test::chain.cpp                                                          //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////
#include <ostream>
#include <iterator>
#include <vector>
#include <boost/array.hpp>
#include <boost/typeof/typeof.hpp>
#include <boost/range/chain.hpp>
#include <boost/next_prior.hpp>
#include <boost/range/algorithm/copy.hpp>
#include <boost/assign/auto_size/ref_list_of.hpp>
#include <boost/assign/auto_size/chain_auto_convert.hpp>
#include <boost/assign/auto_size/chain/mpg.hpp>
#include <libs/assign/example/chain.h>

void example_chain(std::ostream& os)
{

    //boost::assign::detail::chain_mpl_check::compound();
 
    using namespace boost::assign;
    using namespace boost::assign::detail;
	os << "-> example_chain: " << std::endl;
    typedef int val_;
	const val_ a = 1, b = 2, c = 3, d = 4, e = 5, f = 6, g = 7, h = 8, i = 9 ;
    typedef boost::array<val_,3> ar_;
    typedef std::vector<val_> vec_;
    ar_ ar1; ar1.assign( -1 );
    ar_ ar2; ar2.assign( 0 );
    ar_ ar3; ar3.assign( 1 );
    ar_ ar4; ar4.assign( 2 );
	BOOST_AUTO(tmp1,ref_list_of(a)(b)(c)(d)); 
	BOOST_AUTO(tmp2,cref_list_of(e)(f)(g)(h)); 
       
    const int shift = 0;

    // os << boost::mpg::chain(tmp1)(ar2);

    boost::copy(
         chain_auto_convert(
            ref_list_of(a)(b)(c)(d),
            chain_auto_convert(
               cref_list_of(e)(f)(g)(h),
               ar1
            )
        ),
        std::ostream_iterator<val_>(os," ")
    ); 
    os << "nested - rvalues : OK" << std::endl;

    boost::copy(
         chain_auto_convert(
            tmp1,
            chain_auto_convert(
               tmp2,
               ar1
            )
        ),
        std::ostream_iterator<val_>(os," ")
    ); 
    os << "nested - lvalues : OK" << std::endl;

    //boost::copy(
    //    chain_auto_convert(
    //        ref_list_of(a)(b)(c)(d)
    //        ,cref_list_of(e)(f)(g)(h)
    //        ,ar1
    //    ),
    //    std::ostream_iterator<val_>(os," ")
    //); // EXC_BAD_ACCESS
    os << "flattened - rvalues : EXC_BAD_ACCESS" << std::endl;
    
    boost::copy(
        chain_auto_convert(
            tmp1
            ,tmp2
            ,ar1
        ),
        std::ostream_iterator<val_>(os," ")
    );
    os << "flattened - lvalues : OK" << std::endl;


	os << "<- " << std::endl;
    
}
