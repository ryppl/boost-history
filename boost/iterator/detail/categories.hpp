// (C) Copyright Thomas Witt 2002. Permission to copy, use, modify,
// sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#ifndef BOOST_ITERATOR_DETAIL_CATEGORIES_HPP
#define BOOST_ITERATOR_DETAIL_CATEGORIES_HPP

#include <boost/config.hpp>
#include <boost/iterator/detail/config_def.hpp>

#include <boost/detail/workaround.hpp>

#include <boost/type_traits/is_base_and_derived.hpp>
#include <boost/type_traits/is_convertible.hpp>
#include <boost/type_traits/is_same.hpp>

#include <boost/mpl/if.hpp>
#include <boost/mpl/apply_if.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/or.hpp>
#include <boost/mpl/and.hpp>

#include <iterator>

namespace boost
{

  // faked new old-style categories needed to make new->old mapping
  // work
  namespace detail
  {
    struct null_category_tag {};
    struct input_output_iterator_tag : std::input_iterator_tag, std::output_iterator_tag {};
  }
  
  //
  // Access Categories
  //
  struct readable_iterator_tag
  {
      typedef std::input_iterator_tag max_category;
  };
  
  struct writable_iterator_tag
  {
      typedef std::output_iterator_tag max_category;
  };
  
  struct swappable_iterator_tag
  {
      typedef detail::null_category_tag max_category;
  };
  
  struct readable_writable_iterator_tag
    : virtual readable_iterator_tag
    , virtual writable_iterator_tag
    , virtual swappable_iterator_tag
  {
      typedef detail::input_output_iterator_tag max_category;
  };
  
  struct readable_lvalue_iterator_tag
    : virtual readable_iterator_tag
  {
      typedef std::random_access_iterator_tag max_category;
  };
  
  struct writable_lvalue_iterator_tag
    : virtual public readable_writable_iterator_tag
    , virtual public readable_lvalue_iterator_tag
  {
      typedef std::random_access_iterator_tag max_category;
  };

  //
  // Traversal Categories
  //
  struct incrementable_iterator_tag
  {
      typedef std::output_iterator_tag max_category;
  };
  
  struct single_pass_iterator_tag
    : incrementable_iterator_tag
  {
      typedef detail::input_output_iterator_tag max_category;
  };
  
  struct forward_traversal_tag
    : single_pass_iterator_tag
  {
      typedef std::forward_iterator_tag max_category;
  };
  
  struct bidirectional_traversal_tag
    : forward_traversal_tag
  {
      typedef std::bidirectional_iterator_tag max_category;
  };
  
  struct random_access_traversal_tag
    : bidirectional_traversal_tag
  {
      typedef std::random_access_iterator_tag max_category;
  };

  struct error_iterator_tag { };

  namespace detail
  {
    //
    // Tag detection meta functions
    //
    
    // I bet this is defined somewhere else. Let's wait and see.
    struct error_type;

#ifndef BOOST_NO_IS_CONVERTIBLE
    template <class Base, class Derived>
    struct is_same_or_derived
# ifdef BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
      : mpl::or_<
            is_same<Base, Derived>
          , is_base_and_derived<Base, Derived>
        >
    {};
# else
      : is_base_and_derived<Base, Derived>
    {};

    template <class T> struct is_same_or_derived<T,T> : mpl::true_ {};
# endif
    
    template <class Tag, class T>
    struct is_tag
      : mpl::or_<
# ifdef BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
            is_same_or_derived<Tag, T>
# else
            is_base_and_derived<Tag, T>
# endif 

            // Because we can't actually get forward_iterator_tag to
            // derive from input_output_iterator_tag, we need this
            // case.
          , mpl::and_<
                is_same_or_derived<Tag,detail::input_output_iterator_tag>
              , is_same_or_derived<std::forward_iterator_tag,T>
            >
        >
    {};

    
#else 
    template <class Tag, class T>
    struct is_tag;
#endif
    
    // Generate specializations which will allow us to find
    // null_category_tag as a minimum old-style category for new-style
    // iterators which don't have an actual old-style category.  We
    // need that so there is a valid base class for all new-style
    // iterators.
#define BOOST_OLD_ITERATOR_CATEGORY(category)                   \
    template <>                                                 \
    struct is_tag <detail::null_category_tag, std::category>    \
        : mpl::true_ {};

    BOOST_OLD_ITERATOR_CATEGORY(input_iterator_tag)
    BOOST_OLD_ITERATOR_CATEGORY(output_iterator_tag)
    BOOST_OLD_ITERATOR_CATEGORY(forward_iterator_tag)
    BOOST_OLD_ITERATOR_CATEGORY(bidirectional_iterator_tag)
    BOOST_OLD_ITERATOR_CATEGORY(random_access_iterator_tag)
#undef BOOST_OLD_ITERATOR_CATEGORY
        
    template <>
    struct is_tag<detail::input_output_iterator_tag,std::forward_iterator_tag>
      : mpl::true_
    {
    };

#ifndef BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
    template <class T>
    struct is_tag<T,T> : mpl::true_
    {};
    
# ifdef BOOST_NO_IS_CONVERTIBLE
    // Workarounds for CWPro7 which can't detect derivation at
    // compile-time.

    // Fact of life: we can only detect tag refinement relationships
    // among predefined tags.
    //
    // Algorithm:
    //    is_tag(T,U) ->
    //           T == U
    //        || (exists d in derived_from(T) such that is_tag(d, U))
    //
    // T == U case is handled above

    // false by default
    template <class Tag, class T>
    struct is_tag_impl : mpl::false_
    {};

    // The generalized template dispatches to is_tag_impl because
    // is_tag<T,T> and is_tag<some_tag,T> are equally specialized.
    // This technique simulates making is_tag<T,T> more-specialized.
    template <class Tag, class T>
    struct is_tag
      : is_tag_impl<Tag, T>
    {};

# define BOOST_ITERATOR_DERIVED_TAG1(base, derived)     \
    BOOST_ITERATOR_DERIVED_TAG1_AUX(base, _, derived)

# define BOOST_ITERATOR_DERIVED_TAG1_AUX(base, underscore, derived) \
    template<class T>                                               \
    struct is_tag_impl<base##underscore##tag, T>                    \
        : is_tag<derived##underscore##tag, T>                       \
    {                                                               \
    };

    // Old-style tag relations
    template<class T>
    struct is_tag_impl<detail::null_category_tag, T>
      : mpl::or_<
            is_tag<std::output_iterator_tag, T>
          , is_tag<std::input_iterator_tag, T>
        >
    {
    };

    BOOST_ITERATOR_DERIVED_TAG1(std::output_iterator, detail::input_output_iterator)
    BOOST_ITERATOR_DERIVED_TAG1(std::input_iterator, detail::input_output_iterator)
    BOOST_ITERATOR_DERIVED_TAG1(detail::input_output_iterator, std::forward_iterator)
    BOOST_ITERATOR_DERIVED_TAG1(std::forward_iterator, std::bidirectional_iterator)
    BOOST_ITERATOR_DERIVED_TAG1(std::bidirectional_iterator, std::random_access_iterator)
        
    // Access tag relations
    BOOST_ITERATOR_DERIVED_TAG1(readable_lvalue_iterator, writable_lvalue_iterator)
    BOOST_ITERATOR_DERIVED_TAG1(swappable_iterator, readable_writable_iterator)
    BOOST_ITERATOR_DERIVED_TAG1(readable_writable_iterator, writable_lvalue_iterator)

    template<class T>
    struct is_tag_impl<readable_iterator_tag, T>
      : mpl::or_<
            is_tag<readable_lvalue_iterator_tag, T>
          , is_tag<readable_writable_iterator_tag, T>
        >
    {
    };

    BOOST_ITERATOR_DERIVED_TAG1(writable_iterator, readable_writable_iterator)

    // Traversal tag relations
    BOOST_ITERATOR_DERIVED_TAG1(bidirectional_traversal, random_access_traversal)
    BOOST_ITERATOR_DERIVED_TAG1(forward_traversal, bidirectional_traversal)
    BOOST_ITERATOR_DERIVED_TAG1(single_pass_iterator, forward_traversal)
    BOOST_ITERATOR_DERIVED_TAG1(incrementable_iterator, single_pass_iterator)

# endif // BOOST_NO_IS_CONVERTIBLE workarounds
    
#else // BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION case
    
    
#endif 

        
    //
    // Returns the minimum category type or error_type
    // if T1 and T2 are unrelated.
    //
    // For compilers not supporting is_convertible this only
    // works with the new boost return and traversal category
    // types. The exact boost _types_ are required. No derived types
    // will work. 
    //
    //
    template <class T1, class T2>
    struct minimum_category
      : mpl::apply_if<
            is_tag<T1,T2>
          , mpl::identity<T1>
          , mpl::if_<
                is_tag<T2, T1>
              , T2
              , error_type
            >
        >
    {};
    
# if BOOST_WORKAROUND(BOOST_MSVC, <= 1200)
    // Deal with ETI
    template <> struct minimum_category<int, int> { typedef minimum_category type; };
# endif

  } // namespace detail

} // namespace boost

#include <boost/iterator/detail/config_undef.hpp>

#endif // BOOST_ITERATOR_DETAIL_CATEGORIES_HPP
