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
#include <boost/assign/auto_size/chain/add_const.hpp>
#include <boost/assign/auto_size/chain_auto_convert.hpp>
#include <boost/assign/auto_size/chain/mpl_check.hpp>
#include <libs/assign/example/chain.h>

void example_chain(std::ostream& os)
{

    boost::assign::detail::chain_mpl_check::compound();
 
    using namespace boost::assign;
    using namespace boost::assign::detail;
	os << "-> test_chain: " << std::endl;
    typedef int val_;
	val_ a = 1, b = 2, c = 3, d = 4, e = 5, f = 6, g = 7, h = 8, i = 9 ;
    typedef boost::array<val_,3> ar_;
    typedef std::vector<val_> vec_;
    ar_ ar; ar.assign( -1 );
	BOOST_AUTO(tmp1,ref_list_of(a)(b)(c)(d)); 
	BOOST_AUTO(tmp2,cref_list_of(e)(f)(g)(h)); 
    BOOST_AUTO(tmp3,
         tmp1.chain_auto_convert(tmp2));
        //detail::add_const(tmp1).chain_auto_convert(tmp2)); 
    os << *boost::next(boost::begin(tmp3),0) << std::endl;// EXC_BAD_ACCESS if add_const 
    os << *boost::next(boost::begin(tmp3),1) << std::endl;
    os << *boost::next(boost::begin(tmp3),2) << std::endl;
    os << *boost::next(boost::begin(tmp3),3) << std::endl;
    os << *boost::next(boost::begin(tmp3),4) << std::endl;
    os << *boost::next(boost::begin(tmp3),5) << std::endl;
    os << *boost::next(boost::begin(tmp3),6) << std::endl;
    os << *boost::next(boost::begin(tmp3),7) << std::endl;
    
	os << "<- " << std::endl;
    
}
