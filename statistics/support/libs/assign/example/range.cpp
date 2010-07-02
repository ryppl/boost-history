//////////////////////////////////////////////////////////////////////////////
// range.cpp                                                                 //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////
#include <utility>
#include <map>

#include <ostream>
#include <iterator>
#include <vector>
#include <boost/array.hpp>
#include <boost/typeof/typeof.hpp>
#include <boost/next_prior.hpp>
#include <boost/range/algorithm/copy.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/assign/auto_size/ref_list_of.hpp>
#include <boost/assign/auto_size/ref_csv.hpp>
#include <boost/assign/auto_size/range/basic_chain.hpp>
#include <boost/assign/auto_size/range/chain_l.hpp>
#include <boost/assign/auto_size/range/chain_r.hpp>
#include <boost/assign/auto_size/range/convert_range.hpp>

#include <boost/assign/auto_size/detail/repeat.hpp>

// #include <boost/assign/csv.hpp>

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
{

   // [Boost.Assign] list_of::range without an initial	(single) element?
    boost::array<int, 2> a = { 1, 2 };
    boost::array<int, 2> b = { 3, 4 };
    boost::array<int, 4> c = a && b;
    
    BOOST_ASSERT( c[0]==1 );
    BOOST_ASSERT( c[1]==2 );    
    BOOST_ASSERT( c[2]==3 );    
    BOOST_ASSERT( c[3]==4 );
    BOOST_ASSERT( c.size() == 4 );
    
    os << "array : OK" << std::endl;
}
{
   // make<std::vector>(1,2,3,4,5); // Makes a std::vector<int> containing 1,2,3,4,5

    list_of(1)(2)(3)(4)(5).convert_to_container<std::vector<int> >();
    cref_csv(1,2,3,4,5).convert_to_container<std::vector<int> >();
    std::vector<int> vec = cref_csv(1,2,3,4,5);
    BOOST_ASSERT( vec[0]==1 );
    BOOST_ASSERT( vec[1]==2 );    
    BOOST_ASSERT( vec[2]==3 );    
    BOOST_ASSERT( vec[3]==4 );
    BOOST_ASSERT( vec[4]==5 );
    BOOST_ASSERT( vec.size() == 5 );
 
}
{
    using namespace std;
    typedef std::map<int,int> map_;
    typedef std::pair<const int,int> pair_;
    map_ map1 = cref_csv( pair_(1,2), pair_(2,3) );
    BOOST_ASSERT( map1[1] == 2 );
    BOOST_ASSERT( map1[2] == 3 );
    BOOST_ASSERT( map1.size() == 2 );
}
{
    typedef boost::tuple<int,std::string,int> tuple_;

    std::vector<tuple_> v = cref_csv( tuple_(1, "foo", 2) , tuple_(3, "bar", 4) );
    BOOST_ASSERT( v.size() == 2 );
    BOOST_ASSERT( boost::get<0>( v[0] ) ==  1 );
    BOOST_ASSERT( boost::get<1>( v[0] ) ==  "foo" );
    BOOST_ASSERT( boost::get<2>( v[0] ) ==  2 );
    BOOST_ASSERT( boost::get<0>( v[1] ) ==  3 );
    BOOST_ASSERT( boost::get<1>( v[1] ) ==  "bar" );
    BOOST_ASSERT( boost::get<2>( v[1] ) ==  4 );
}
{
    std::vector<int> vec = (list_of(1),2,3);
    BOOST_ASSERT(vec[0]==1);    
    BOOST_ASSERT(vec[1]==2);    
    BOOST_ASSERT(vec[2]==3);    
}
{
    int a = 4;
    BOOST_AUTO( tmp, repeat<3>(a) );
    BOOST_ASSERT( tmp[0] == 4 );    
    BOOST_ASSERT( tmp[1] == 4 );    
    BOOST_ASSERT( tmp[2] == 4 );    
    BOOST_ASSERT( tmp.size() == 3 );
    tmp[0] = 1;
    BOOST_ASSERT( a == 1 );
    tmp[1] = 2;
    BOOST_ASSERT( a == 2 );
    tmp[2] = 3;
    BOOST_ASSERT( a == 3 );
}
{
    int a = 1, b = 2 ,c = 3;
    std::vector<int> vec = list_of(a).csv(b,c);
    BOOST_ASSERT( vec[0] == 1 );    
    BOOST_ASSERT( vec[1] == 2 );    
    BOOST_ASSERT( vec[2] == 3 );    
}
{
    int a = 4;
    BOOST_AUTO( tmp, c_repeat<3>(a) );
    BOOST_ASSERT( tmp[0] == 4 );    
    BOOST_ASSERT( tmp[1] == 4 );    
    BOOST_ASSERT( tmp[2] == 4 );    
    BOOST_ASSERT( tmp.size() == 3 );
}

	os << "<- " << std::endl;
    
}