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
#include <boost/assign/auto_size/reference_wrapper/chain_traits.hpp> // MUST be included
#include <boost/assign/chain/auto_convert.hpp>
#include <libs/assign/example/chain.h>

void example_chain(std::ostream& os)
{
    using namespace boost::assign;
    using namespace boost::assign::detail;
	os << "-> test_chain: " << std::endl;
    typedef int val_;
	val_ a = 1, b = 2, c = 3;
    typedef boost::array<val_,3> ar_;
    typedef std::vector<val_> vec_;
    typedef detail::result_of::convert_range<val_&,ar_> caller_;
    typedef caller_::type res_;
    ar_ ar; ar.assign(-1);
	BOOST_AUTO(tmp1,cref_list_of(a)(b)(c)); 
	boost::copy(
        detail::chain_auto_convert_filter_wrapper(tmp1,ar),
        std::ostream_iterator<val_>(os," ")
    );

	os << "<- " << std::endl;
    
}
