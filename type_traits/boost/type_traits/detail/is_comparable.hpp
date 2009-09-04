#include <boost/config.hpp>
#include <boost/type_traits/remove_cv.hpp>
#include <boost/type_traits/integral_constant.hpp>

// should be the last #include
#include <boost/type_traits/detail/bool_trait_def.hpp>

namespace boost {
namespace detail {

// This namespace ensures that ADL doesn't mess things up.
namespace BOOST_JOIN(BOOST_TT_TRAIT_NAME,_impl) {

// a type returned from comparison operator when no such operator is found in the
// type's own namespace
struct tag { } ;

// any soaks up implicit conversions and makes the following
// comparison operators less-preferred than any other such operators that
// might be found via ADL.
struct any { template <class T> any(T const&) ; } ;

tag operator BOOST_TT_TRAIT_OP (const any&, const any&) ;

// In case a comparison operator is found that returns void, we'll use (x comp x),0
tag operator,(tag, int) ;

// two check overloads help us identify which comparison operator was picked

// check(tag) returns a reference to char[2] (sizeof>1)
char (& check(tag))[2] ;

// check(not tag) returns char (sizeof==1)
template <class T> char check(T const&) ;

template <class T>
struct BOOST_JOIN(BOOST_TT_TRAIT_NAME,_impl) {
	static typename boost::remove_cv<T>::type &x ;
	static const bool value = sizeof(check((x BOOST_TT_TRAIT_OP x, 0))) == 1 ;
} ;

} // namespace impl
} // namespace detail

BOOST_TT_AUX_BOOL_TRAIT_DEF1(BOOST_TT_TRAIT_NAME,T,::boost::detail::BOOST_JOIN(BOOST_TT_TRAIT_NAME,_impl)::BOOST_JOIN(BOOST_TT_TRAIT_NAME,_impl)<T>::value)

} // namespace boost

#include <boost/type_traits/detail/bool_trait_undef.hpp>
