// Dinkumware standard library config:
#if !defined(_YVALS) && !defined(_CPPLIB_VER)
#include <utility>
#if !defined(_YVALS) && !defined(_CPPLIB_VER)
#error This is not the Dinkumware lib!
#endif
#endif


#if defined(_CPPLIB_VER) && (_CPPLIB_VER >= 306)
   // full dinkumware 3.06 and above
   // fully conforming provided the compiler supports it:
#  if !(defined(_GLOBAL_USING) && (_GLOBAL_USING > 0))    // can be defined in yvals.h
#     define BOOST_NO_STDC_NAMESPACE
#  endif
// 3.06 appears to have (non-sgi versions of) <hash_set> & <hash_map>, but no <slist>
//#  define BOOST_HAS_SLIST
#else
#  define BOOST_MSVC_STD_ITERATOR 1
#  define BOOST_NO_STD_ITERATOR
#  define BOOST_NO_TEMPLATED_ITERATOR_CONSTRUCTORS
#  define BOOST_NO_STD_ALLOCATOR
#  define BOOST_NO_STDC_NAMESPACE
#  define BOOST_NO_STD_USE_FACET
#  define BOOST_HAS_MACRO_USE_FACET
#  ifndef _CPPLIB_VER
      // Updated Dinkum library defines this, and provides
      // its own min and max definitions.
#     define BOOST_NO_STD_MIN_MAX
#     undef min
#     undef max
#  endif
#  ifndef NOMINMAX
      // avoid spurious NOMINMAX redefinition warning
#     define NOMINMAX
#  endif
#endif

#if defined(_MSC_VER) && (_MSC_VER <= 1200)
   // if we're using a dinkum lib that's
   // been configured for VC6 then there is
   // no iterator traits (true even for icl)
#  define BOOST_NO_STD_ITERATOR_TRAITS
#endif

#ifdef _CPPLIB_VER
#  define BOOST_STDLIB "Dinkumware standard library version " BOOST_STRINGIZE(_CPPLIB_VER)
#else
#  define BOOST_STDLIB "Dinkumware standard library version 1.x"
#endif


