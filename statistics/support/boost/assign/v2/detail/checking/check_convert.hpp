#include <boost/assign/v2/detail/config/enable_cpp0x.hpp>

// TODO find out why c++0x won't allow to = from;
// Usage:
//  using checking::container; // or maybe checking::array
//  BOOST_ASSIGN_V2_check_convert

#ifndef BOOST_ASSIGN_V2_check_convert
#if BOOST_ASSIGN_V2_ENABLE_CPP0X
#define BOOST_ASSIGN_V2_check_convert do_check( from.convert<to_>() );
#else
#define BOOST_ASSIGN_V2_check_convert \
    do_check( from.convert<to_>() ); \
    to_ to; to = from; \
    do_check( to ); \
/**/
#endif
#endif

