#ifndef BOOST_CUCHAR_HPP
#define BOOST_CUCHAR_HPP

#include <boost/cstdint.hpp>

namespace boost
{
#ifdef BOOST_NO_CHAR16_T
    typedef boost::uint_least16_t char16;
#else
	typedef char16_t char16;
#endif
	
#ifdef BOOST_NO_CHAR32_T
	typedef boost::uint_least32_t char32;
#else
	typedef char32_t char32;
#endif
    
} // namespace boost

#endif
