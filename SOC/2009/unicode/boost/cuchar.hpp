#ifndef BOOST_CUCHAR_HPP
#define BOOST_CUCHAR_HPP

#include <boost/cstdint.hpp>
#include <boost/detail/unspecified.hpp>

namespace boost
{
#if defined(BOOST_NO_CHAR16_T)
    typedef uint_least16_t char16;
#else
    /** Typedef to C++0x char16_t, or uint_least16_t from cstdint if not available. **/
    typedef detail::unspecified<char16_t>::type char16;
#endif
	
#if defined(BOOST_NO_CHAR32_T)
	typedef boost::uint_least32_t char32;
#else
    /** Typedef to C++0x char32_t, or uint_least32_t from cstdint if not available. **/
    typedef detail::unspecified<char32_t>::type char32;
#endif
    
} // namespace boost

#endif
