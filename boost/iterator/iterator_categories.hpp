// (C) Copyright Jeremy Siek 2002. Permission to copy, use, modify,
// sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

// TODO:
//   Add separate category tag for operator[].

#ifndef BOOST_ITERATOR_CATEGORIES_HPP
#define BOOST_ITERATOR_CATEGORIES_HPP

#include <boost/config.hpp>
#include <boost/iterator/detail/categories.hpp>
#include <boost/type_traits/conversion_traits.hpp>
#include <boost/type_traits/cv_traits.hpp>
#include <boost/detail/iterator.hpp>
#include <boost/detail/workaround.hpp>
#include <boost/mpl/apply_if.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/bool_c.hpp>
#include <boost/mpl/aux_/has_xxx.hpp>
#include <boost/mpl/logical/not.hpp>
#include <boost/mpl/logical/or.hpp>
#include <boost/mpl/logical/and.hpp>
#include <iterator>

#if BOOST_WORKAROUND(__MWERKS__, <=0x2407)
#  define BOOST_NO_IS_CONVERTIBLE // "Convertible does not provide enough/is not working"
#endif

namespace boost {

  namespace detail
  {

    template <typename ValueType>
    struct choose_lvalue_return
        : mpl::if_<
            is_const<ValueType>
            , boost::constant_lvalue_iterator_tag
            , boost::mutable_lvalue_iterator_tag>
    {
    };
    
    
    template <typename Category, typename ValueType>
    struct iter_category_to_return
        : mpl::if_<
           is_forward_iterator<Category>
           , typename choose_lvalue_return<ValueType>::type
           , typename mpl::if_<
                is_input_iterator<Category>
                , boost::readable_iterator_tag
                , typename mpl::if_<
                    is_output_iterator<Category>
                    , boost::writable_iterator_tag
                    , boost::error_iterator_tag
                  >::type
             >::type
         >
    {
    };

    template <typename Category>
    struct iter_category_to_traversal
      : mpl::if_<
          is_random_access_iterator<Category>
          , random_access_traversal_tag
          , typename mpl::if_<
              is_bidirectional_iterator<Category>
              , bidirectional_traversal_tag
              , typename mpl::if_<
                  is_forward_iterator<Category>
                  , forward_traversal_tag
                  , typename mpl::if_<
                      is_input_iterator<Category>
                      , input_traversal_tag
                      , output_traversal_tag
                    >::type
                >::type
            >::type
        >
    {
    };

    template <class T>
    struct is_boost_iterator_tag;

#if BOOST_WORKAROUND(__MWERKS__, <= 0x2407)
    //
    // has_xxx fails, so we have to use 
    // something less sophisticated.
    // 
    // The solution depends on the fact that only
    // std iterator categories work with is_xxx_iterator
    // meta functions, as BOOST_NO_IS_CONVERTIBLE is
    // defined for cwpro7.
    //
    template <class Tag>
    struct is_new_iterator_tag :
      mpl::logical_and< mpl::logical_not< is_input_iterator<Tag> >,
                        mpl::logical_not< is_output_iterator<Tag> > >
    {};

#elif BOOST_WORKAROUND(__GNUC__, == 2 && __GNUC_MINOR__ == 95) \
   || BOOST_WORKAROUND(__BORLANDC__, BOOST_TESTED_AT(0x551))
    
    template <class Tag>
    struct is_new_iterator_tag
        : is_boost_iterator_tag<Tag>
    {
    };
    
#else
    
  BOOST_MPL_HAS_XXX_TRAIT_DEF(traversal)

    template <class Tag>
    struct is_new_iterator_tag
        : //has_traversal<Tag>
          mpl::if_<
          is_class<Tag>
          , has_traversal<Tag>
          , mpl::bool_c<false> >::type
    {
    };

#endif

  } // namespace detail
  
  namespace detail {

    template <class NewCategoryTag>
    struct get_return_category {
      typedef typename NewCategoryTag::returns type;
    };
    template <class NewCategoryTag>
    struct get_traversal_category {
      typedef typename NewCategoryTag::traversal type;
    };

    template <class CategoryTag, class Value>
    struct return_category_tag
        : mpl::apply_if< 
           is_new_iterator_tag<CategoryTag>
           , get_return_category<CategoryTag>
           , iter_category_to_return<CategoryTag, Value>
    >
    {
    };
  
    template <class CategoryTag, class Value>
    struct traversal_category_tag
        : mpl::apply_if< 
	  is_new_iterator_tag<CategoryTag>
	  , get_traversal_category<CategoryTag>
	  , iter_category_to_traversal<CategoryTag>
	>
    {
    };

  } // namespace detail

  template <class Iterator>
  struct return_category
      : detail::return_category_tag<
           typename detail::iterator_traits<Iterator>::iterator_category
          , typename detail::iterator_traits<Iterator>::value_type>
  {};

  template <class Iterator>
  struct traversal_category
      : detail::traversal_category_tag<
          typename detail::iterator_traits<Iterator>::iterator_category
         , typename detail::iterator_traits<Iterator>::value_type>
  {
  };

#if !defined(BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION)

  template <typename T>
  struct return_category<T*>
      : mpl::if_<
          is_const<T>
          , constant_lvalue_iterator_tag
          , mutable_lvalue_iterator_tag>
  {
  };

  template <typename T>
  struct traversal_category<T*>
  {
    typedef random_access_traversal_tag type;
  };

#endif

  // TODO Fix this for BOOST_NO_IS_CONVERTIBLE

  template <class RC, class TC>
  struct cvt_iterator_category
      : mpl::if_<
          mpl::logical_or<
             detail::is_mutable_lvalue_iterator<RC>
             , detail::is_constant_lvalue_iterator<RC>
          >
          , typename mpl::if_<
              detail::is_random_access_traversal_iterator<TC>
              , std::random_access_iterator_tag
              , typename mpl::if_<
                  detail::is_bidirectional_traversal_iterator<TC>
                  , std::bidirectional_iterator_tag
                  , typename mpl::if_<
                      detail::is_forward_traversal_iterator<TC>
                      , std::forward_iterator_tag
                      , error_iterator_tag
                    >::type
                  >::type
                >::type

          , typename mpl::if_<
            mpl::logical_and<
               detail::is_readable_iterator<RC>
               , detail::is_input_traversal_iterator<TC>
            >
            , std::input_iterator_tag
            , typename mpl::if_<
                mpl::logical_and<
                  detail::is_writable_iterator<RC>
                  , detail::is_output_traversal_iterator<TC>
                >
                , std::output_iterator_tag
                , error_iterator_tag
             >::type
          >::type 
    >
  {
  };

  template <class ReturnTag, class TraversalTag>
  struct iterator_tag : cvt_iterator_category<ReturnTag, TraversalTag>::type
  {
    typedef ReturnTag returns;
    typedef TraversalTag traversal;
  };

  namespace detail
  {
# ifndef BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
    template <class T>
    struct is_boost_iterator_tag
        : mpl::false_c {};

    template <class R, class T>
    struct is_boost_iterator_tag<iterator_tag<R,T> >
        : mpl::true_c {};
# else
    template <class T>
    struct is_boost_iterator_tag_impl
    {
        typedef char (&yes)[1];
        typedef char (&no)[2];
        
        template <class R, class T>
        static yes test(iterator_tag<R,T> const&);
        static no test(...);
    
        static T inst;
        BOOST_STATIC_CONSTANT(bool, value = sizeof(test(inst)) == sizeof(yes));
    };

    template <class T>
    struct is_boost_iterator_tag
        : mpl::bool_c<is_boost_iterator_tag_impl<T>::value>
    {};
# endif
  }

} // namespace boost

#undef BOOST_NO_IS_CONVERTIBLE

#endif // BOOST_ITERATOR_CATEGORIES_HPP
