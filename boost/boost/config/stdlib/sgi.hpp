// generic SGI STL:
#if !defined(__STL_CONFIG_H)
#  include <utility>
#  if !defined(__STL_CONFIG_H)
#      error "This is not the SGI STL!"
#  endif
#endif

//
// no std::iterator traits without partial specialisation:
//
#if !defined(__STL_CLASS_PARTIAL_SPECIALIZATION)
#  define BOOST_NO_STD_ITERATOR_TRAITS
#endif

//
// No std::stringstream with gcc < 3
//
#if (defined(__GNUC__) && (__GNUC__ < 3)) && !defined(__STL_USE_NEW_IOSTREAMS)
#  define BOOST_NO_STRINGSTREAM
#endif

//
// Assume no std::locale without own iostreams (this may be an
// incorrect assumption in some cases):
//
#ifndef __SGI_STL_OWN_IOSTREAMS
#  define BOOST_NO_STD_LOCALE
#endif

//
// No template iterator constructors, or std::allocator
// without member templates:
//
#if !defined(__STL_MEMBER_TEMPLATES)
#  define BOOST_NO_TEMPLATED_ITERATOR_CONSTRUCTORS
#  define BOOST_NO_STD_ALLOCATOR
#endif

//
// we always have SGI style hash_set, hash_map, and slist:
//
#define BOOST_HAS_HASH
#define BOOST_HAS_SLIST

//
// If this is GNU libstdc++2, then no <limits> and no std::wstring:
//
#if (defined(__GNUC__) && (__GNUC__ < 3))
#  include <string>
#  if defined(__BASTRING__)
#     define BOOST_NO_LIMITS
#     define BOOST_NO_STD_WSTRING
#  endif
#endif

//
// there is no standard iterator unless we have namespace support
// or the platform is BeOS/GNUPro which has a modified header:
//
#if !defined(__STL_USE_NAMESPACES) && !(defined(__BEOS__) && defined(__BASTRING__))
#  define BOOST_NO_STD_ITERATOR
#endif

#define BOOST_STDLIB "SGI standard library"


