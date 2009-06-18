#ifndef BOOST_CUCHAR_HPP
#define BOOST_CUCHAR_HPP

#include <boost/cstdint.hpp>

namespace boost
{
#if defined(BOOST_NO_CHAR16_T)
    typedef boost::uint_least16_t char16;
#elif defined(BOOST_UNICODE_DOXYGEN_INVOKED)
    /** Typedef to C++0x char16_t, or uint_least16_t from cstdint if not available. **/
    typedef detail::char16 char16;
#else
	typedef char16_t char16;
#endif
	
#if defined(BOOST_NO_CHAR32_T)
	typedef boost::uint_least32_t char32;
#elif defined(BOOST_UNICODE_DOXYGEN_INVOKED)
    /** Typedef to C++0x char32_t, or uint_least32_t from cstdint if not available. **/
    typedef detail::char32 char32;
#else
	typedef char32_t char32;
#endif
    
} // namespace boost

#endif
