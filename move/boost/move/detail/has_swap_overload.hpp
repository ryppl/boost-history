// Copyright David Abrahams 2004. Use, modification and distribution is
// subject to the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef HAS_SWAP_OVERLOAD_DWA200415_HPP
# define HAS_SWAP_OVERLOAD_DWA200415_HPP

# include <boost/type_traits/detail/template_arity_spec.hpp>
# include <boost/type_traits/remove_cv.hpp>
# include <boost/mpl/aux_/lambda_support.hpp>
# include <boost/mpl/bool.hpp>
# include <boost/detail/workaround.hpp>

// Must be the last include
# include <boost/type_traits/detail/bool_trait_def.hpp>

namespace std
{
  // Defining names in std is illegal; so sue me
  //
  // This allows us to test whether std is an associated namespace of
  // the type in question.  If it is, we have to assume any swap
  // overload we detect is std::swap and that there is no
  // type-specific swap overload.  This is the conservative default.
  // We'll specialize has_swap_overload for such known-swappables as
  // std::vector.
  template <class T>
  char stdIsAnAssociatedNamespace(T const&, int);

  // Forward declare some known types that have a fast swap.  Also
  // illegal last I checked.
  template <class T, class A> class vector;
  template <class T, class A> class deque;
  template <class T, class A> class list;
  template <class T, class A, class C> class set;
  template <class T, class U, class C, class A> class map;
}

namespace boost { namespace detail { 

// has_swap_overload<T> metafunction
//
// Requires: Given x of type T&, if the expression swap(x,x) is
// well-formed it must have complete type or void result; otherwise,
// it must neither be ambiguous nor violate access.

// This namespace ensures that ADL doesn't mess things up.
namespace has_swap_overload_
{
  // a type returned from swap() when no increment is found in the
  // type's own namespace
  struct tag {};
  
  // any soaks up implicit conversions and makes the following
  // swap less-preferred than any other such function that
  // might be found via ADL.
  struct any { template <class T> any(T const&); };

  // This is a last-resort swap() for when none other is found
# if BOOST_WORKAROUND(__GNUC__, == 4) && __GNUC_MINOR__ == 0 && __GNUC_PATCHLEVEL__ == 2
  
}

namespace has_swap_overload_2
{
  has_swap_overload_::tag swap(has_swap_overload_::any const&, has_swap_overload_::any const&);
}
using namespace has_swap_overload_2;

namespace has_swap_overload_
{
  
# else
  
  tag swap(any const&, any const&);
  
# endif 

# if BOOST_WORKAROUND(__MWERKS__, BOOST_TESTED_AT(0x3202)) \
    || BOOST_WORKAROUND(BOOST_MSVC, <= 1300)
#  define BOOST_comma(a,b) (a)
# else 
  // In case a swap() is found that returns void (likely), we'll use swap(x,x),0
  tag operator,(tag,int);  
#  define BOOST_comma(a,b) (a,b)
# endif 

# if defined(BOOST_MSVC)
#  pragma warning(push)
#  pragma warning(disable:4913) // Warning about operator,
# endif 

  typedef char yes;
  typedef char (&no)[2];
  
  // two check overloads help us identify which swap was picked
  no check(tag);
  
  template <class T>
  yes check(T const&);
  
  template <class T>
  no stdIsAnAssociatedNamespace(T const&, ...);

  template <class T>
  struct impl
  {
      static typename boost::remove_cv<T>::type& x;

      BOOST_STATIC_CONSTANT(
          bool
        , value = sizeof(stdIsAnAssociatedNamespace(x, 1)) == sizeof(no)
          && sizeof(has_swap_overload_::check(BOOST_comma(swap(x,x),0))) == sizeof(yes)
      );
  };

# if defined(BOOST_MSVC)
#  pragma warning(pop)
# endif 

}

# undef BOOST_comma

template<typename T> 
struct has_swap_overload 
BOOST_TT_AUX_BOOL_C_BASE(::boost::detail::has_swap_overload_::impl<T>::value)
{ 
    BOOST_TT_AUX_BOOL_TRAIT_VALUE_DECL(::boost::detail::has_swap_overload_::impl<T>::value)
    BOOST_MPL_AUX_LAMBDA_SUPPORT(1,has_swap_overload,(T))
};


template <class T, class A>
struct has_swap_overload<std::vector<T,A> > : mpl::true_{};

template <class T, class A>
struct has_swap_overload<std::deque<T,A> > : mpl::true_{};

template <class T, class A>
struct has_swap_overload<std::list<T,A> > : mpl::true_{};

template <class T, class A, class C>
struct has_swap_overload<std::set<T,A,C> > : mpl::true_{};

template <class K, class V, class C, class A>
struct has_swap_overload<std::map<K,V,C,A> > : mpl::true_{};

} // namespace detail

BOOST_TT_AUX_TEMPLATE_ARITY_SPEC(1, ::boost::detail::has_swap_overload)

} // namespace boost

# include <boost/type_traits/detail/bool_trait_undef.hpp>

#endif // HAS_SWAP_OVERLOAD_DWA200415_HPP
