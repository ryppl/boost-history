//
// Options common to all edg based compilers.
//
// This is included by <boost/config/suffix.hpp> when
// __EDG_VERSION__ is defined, so there is no need to include
// this from within the individual compiler mini-configs.


#if (__EDG_VERSION__ <= 241) && !defined(BOOST_NO_ARGUMENT_DEPENDENT_LOOKUP)
#   define BOOST_NO_ARGUMENT_DEPENDENT_LOOKUP
#endif

