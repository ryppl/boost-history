#ifndef BOOST_UNICODE_DECL_HPP
#define BOOST_UNICODE_DECL_HPP

#include <boost/config.hpp>

#ifdef BOOST_HAS_DECLSPEC
#if defined(BOOST_ALL_DYN_LINK) || defined(BOOST_UNICODE_DYN_LINK)
// export if this is our own source, otherwise import:
#ifdef BOOST_UNICODE_SOURCE
# define BOOST_UNICODE_DECL __declspec(dllexport)
#else
# define BOOST_UNICODE_DECL __declspec(dllimport)
#endif  // BOOST_UNICODE_SOURCE
#endif  // DYN_LINK
#endif  // BOOST_HAS_DECLSPEC

#ifndef BOOST_UNICODE_DECL
#define BOOST_UNICODE_DECL
#endif

//
// Automatically link to the correct build variant where possible. 
// 
#if !defined(BOOST_ALL_NO_LIB) && !defined(BOOST_UNICODE_NO_LIB) && !defined(BOOST_UNICODE_SOURCE)
//
// Set the name of our library, this will get undef'ed by auto_link.hpp
// once it's done with it:
//
#define BOOST_LIB_NAME boost_unicode
//
// If we're importing code from a dll, then tell auto_link.hpp about it:
//
#if defined(BOOST_ALL_DYN_LINK) || defined(BOOST_UNICODE_DYN_LINK)
#  define BOOST_DYN_LINK
#endif
//
// And include the header that does the work:
//
#include <boost/config/auto_link.hpp>
#endif  // auto-linking disabled

#endif // BOOST_UNICODE_DECL_HPP
