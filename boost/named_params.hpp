// Copyright David Abrahams, Daniel Wallin 2003. Use, modification and 
// distribution is subject to the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_NAMED_PARAMS_031014_HPP
#define BOOST_NAMED_PARAMS_031014_HPP

#include <boost/mpl/lambda.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/mpl/always.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/or.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/is_convertible.hpp>
#include <boost/iterator/detail/config_def.hpp>
#include <boost/python/detail/is_xxx.hpp>

#include <boost/preprocessor/repetition/enum.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_trailing_params.hpp>
#include <boost/preprocessor/arithmetic/sub.hpp>
#include <boost/preprocessor/repetition/enum_shifted.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>
#include <boost/preprocessor/repetition/enum_shifted_params.hpp>
#include <boost/preprocessor/iteration/iterate.hpp>
#include <boost/preprocessor/facilities/intercept.hpp>
#include <boost/preprocessor/cat.hpp>

#ifndef BOOST_NAMED_PARAMS_MAX_ARITY
# define BOOST_NAMED_PARAMS_MAX_ARITY 5
#endif

#if defined(__GNUC__) && __GNUC__ < 3
#   define BOOST_NAMED_PARAMS_GCC2 1
#else
#   define BOOST_NAMED_PARAMS_GCC2 0
#endif

namespace boost {

template<class T>
class reference_wrapper;

template<class T>
struct keyword;

template <class Tag, class HasDefault = mpl::true_, class Predicate = mpl::always<mpl::true_> >
struct named_param;

namespace detail
{ 
  // We have to build a conversion operator to get around ETI with
  // default function arguments; int seemed a little too easily
  // confused, so we'll use this special type instead.
  struct non_int_eti_type { private: non_int_eti_type(); };
  
  typedef char yes_t;
  struct no_t { char x[128]; };

  yes_t to_yesno(mpl::true_);
  no_t to_yesno(mpl::false_);

  template <class KW, class Default>
  struct named_default
  {
      named_default(Default& x)
        : default_(x)
      {}

      Default& default_;
  };


#if BOOST_WORKAROUND(__EDG_VERSION__, <= 300)
  // These compilers need a little extra help with overload
  // resolution; we have nil's operator[] accept a base class to make
  // that overload less preferable.
  template <class KW, class DefaultFn>
  struct lazy_named_default_base
  {
      lazy_named_default_base(const DefaultFn& x)
        : default_(x)
      {}
      const DefaultFn& default_;
  };
  
  template <class KW, class DefaultFn>
  struct lazy_named_default
    : lazy_named_default_base<KW,DefaultFn>
  {
      lazy_named_default(const DefaultFn& x)
        : lazy_named_default_base<KW,DefaultFn>(x)
      {}
  };
#else 
  template <class KW, class DefaultFn>
  struct lazy_named_default
  {
      lazy_named_default(const DefaultFn& x)
        : default_(x)
      {}
      const DefaultFn& default_;
  };
#endif 

  template <class KW, class T>
  struct named;
  
  struct nil
  {
      nil() {}
      nil(BOOST_PP_ENUM_PARAMS(
              BOOST_NAMED_PARAMS_MAX_ARITY, nil BOOST_PP_INTERCEPT
      )) {}

#if BOOST_WORKAROUND(BOOST_MSVC, <= 1300) || BOOST_NAMED_PARAMS_GCC2
      // A metafunction class which, given a keyword, returns the base
      // sublist whose get() function can produce the value for that
      // key.
      struct key_owner
      {
          template<class KW>
          struct apply
          {
              typedef nil type;
          };
      };

      // A metafunction class which, given a keyword and a default
      // type, returns the appropriate result type for a keyword
      // lookup given that default
      struct key_value_type
      {
          template<class KW, class Default>
          struct apply
          {
              typedef Default type;
          };
      };

      template <class K, class Default>
      Default& get(const named_default<K, Default>& x) const
      {
          return x.default_;
      }

      template <class K, class Default>
      typename Default::result_type get(
          const lazy_named_default<K, Default>& x) const
      {
          return x.default_();
      }     

      template <class K, class Default>
      Default& operator[](const named_default<K, Default>& x) const
      {
          return x.default_;
      }

      template <class K, class Default>
      typename Default::result_type operator[](
          const lazy_named_default<K, Default>& x) const
      {
          return x.default_();
      }
#else
      template <class K, class Default>
      Default& operator[](const named_default<K, Default>& x) const
      {
          return x.default_;
      }

# if BOOST_WORKAROUND(__EDG_VERSION__, <= 300)
      template <class K, class Default>
      typename Default::result_type operator[](
          const lazy_named_default_base<K, Default>& x) const
      {
          return x.default_();
      }
# else
      template <class K, class Default>
      typename Default::result_type operator[](
          const lazy_named_default<K, Default>& x) const
      {
          return x.default_();
      }
# endif

#endif
      // No keyword was found if we get here, so we should only return
      // mpl::true_ if it's OK to have a default for the argument.
      template <class Arg>
      static typename Arg::has_default keyword_passes_predicate(Arg*);

      typedef mpl::true_ has_default;
      typedef mpl::always<mpl::true_> predicate;
  };

  template <class T>
  struct key_value_type_mfn
  {
      typedef typename T::key_value_type type;
  };

  // A tuple of labeled argument holders
  // Restructured this so that head isn't inherited
  // We'll need a version without using declarations for vc6/7.0
  template <class H, class T = nil>
  struct list : T
  {
      typedef list<H, T> self_t;

#if BOOST_WORKAROUND(BOOST_MSVC, <= 1300) || BOOST_NAMED_PARAMS_GCC2

      typedef H head_type;
      typedef T tail_type;

      // A metafunction class which, given a keyword, returns the base
      // sublist whose get() function can produce the value for that
      // key.
      struct key_owner
      {
          template<class KW>
          struct apply
            : mpl::eval_if<
                  boost::is_same<KW, typename H::key_type>
                , mpl::identity<self_t>
                , mpl::apply1<typename T::key_owner,KW>
              >
          {};
      };

      // A metafunction class which, given a keyword and a default
      // type, returns the appropriate result type for a keyword
      // lookup given that default
      struct key_value_type
      {
          template <class KW, class Default>
          struct apply
            : mpl::eval_if<
                  boost::is_same<KW, typename H::key_type>
                , mpl::identity<typename H::value_type>
                , mpl::apply2<typename T::key_value_type,KW, Default>
              >
          {
          };
      };
#endif
      
      H head;

      list() {}

      template<
          BOOST_PP_ENUM_PARAMS(BOOST_NAMED_PARAMS_MAX_ARITY, class A)
      >
      list(
          BOOST_PP_ENUM_BINARY_PARAMS(BOOST_NAMED_PARAMS_MAX_ARITY, A, const & a)
      )
         : T(BOOST_PP_ENUM_SHIFTED_PARAMS(BOOST_NAMED_PARAMS_MAX_ARITY, a), nil())
         , head(a0)
      {}

      list(H const& head_, T const& tail)
          : T(tail)
          , head(head_)
      {
      }

#if BOOST_WORKAROUND(BOOST_MSVC, <= 1300) || BOOST_NAMED_PARAMS_GCC2
      template <class KW>
      typename mpl::apply2<key_value_type,KW, nil>::type&
      operator[](const keyword<KW>& x) const
      {
          typename mpl::apply1<key_owner,KW>::type const& sublist = *this;
          return sublist.get(x);
      }

      template <class KW, class Default>
      typename mpl::apply2<key_value_type,KW, Default>::type&
      operator[](const named_default<KW, Default>& x) const
      {
          typename mpl::apply1<key_owner,KW>::type const& sublist = *this;
          return sublist.get(x);
      }

      template <class KW, class Default>
      typename mpl::apply2<
          key_value_type,KW
        , typename Default::result_type
      >::type
      operator[](const lazy_named_default<KW, Default>& x) const
      {
          typename mpl::apply1<key_owner,KW>::type const& sublist = *this;
          return sublist.get(x);
      }

      typename H::value_type& get(const keyword<typename H::key_type>& x) const
      {
          return head[x];
      }

      template <class Default>
      typename H::value_type& get(
          const named_default<typename H::key_type, Default>& x) const
      {
          return head[x];
      }

      template <class Default>
      typename H::value_type& get(
          const lazy_named_default<typename H::key_type, Default>& x) const
      {
          return head[x];
      }
#else
      typename H::value_type&
      operator[](const keyword<typename H::key_type>& x) const
      {
          return head[x];
      }

      template <class Default>
      typename H::value_type&
      operator[](const named_default<typename H::key_type, Default>& x) const
      {
          return head[x];
      }

      template <class Default>
      typename H::value_type&
      operator[](const lazy_named_default<typename H::key_type, Default>& x) const
      {
          return head[x];
      }
      
      using T::operator[];

      template <class HasDefault, class Predicate>
      static typename mpl::apply1<
          typename mpl::lambda<Predicate>::type
        , typename H::value_type
      >::type
      keyword_passes_predicate(named_param<typename H::key_type,HasDefault,Predicate>*);

      using T::keyword_passes_predicate;
#endif

      template <class KW, class T2>
      list<named<KW, T2>, self_t>
      operator,(named<KW, T2> const& x) const
      {
          return list<named<KW, T2>, self_t>(
              x, *this
          );
      }
  };

  template <> struct list<int,int> {};

  template <class KW, class T>
  struct named
  {
      typedef KW key_type;
      typedef T value_type;

      named(T& x) : val(x) {}

      T& operator[](const keyword<KW>&) const
      {
          return val;
      }

      template <class Default>
      T& operator[](const named_default<KW, Default>&) const
      {
          return val;
      }

      template <class Default>
      T& operator[](const lazy_named_default<KW, Default>&) const
      {
          return val;
      }

      template <class KW2, class T2>
      list<named<KW, T>, list<named<KW2, T2> > >
      operator,(named<KW2, T2> const& x) const
      {
          return list<named<KW, T>, list<named<KW2, T2> > >(
              *this, list<named<KW2, T2> >(x, nil())
          );
      }

      T& val;
  };

  BOOST_PYTHON_IS_XXX_DEF(named,named,2)

  template <class U>
  yes_t is_const_reference_wrapper_check(const reference_wrapper<U>*);
  no_t is_const_reference_wrapper_check(...);

  // Returns mpl::true_ if T is of type const reference_wrapper<U>
  template <class T>
  struct is_const_reference_wrapper
  {
     BOOST_STATIC_CONSTANT(
         bool, value = (
             sizeof(is_const_reference_wrapper_check((T*)0)) == sizeof(yes_t)
         )
     );

     typedef mpl::bool_<value> type;
  };

  template <class T>
  struct get_type
  {
      typedef typename T::type type;
  };
  
  // Produces the unwrapped type to hold a reference to in named<>
  // Can't use boost::unwrap_reference<> here because it
  // doesn't handle the case where T = const reference_wrapper<U>
  template <class T>
  struct unwrap_cv_reference
  {
      typedef typename mpl::eval_if<
          is_const_reference_wrapper<T>
        , get_type<T>
        , mpl::identity<T>
      >::type type;
  };

} // namespace detail

template <class Tag>
struct keyword
{
#if !BOOST_WORKAROUND(BOOST_MSVC, == 1200)  // partial ordering bug
   template <class T>
   detail::named<Tag, typename detail::unwrap_cv_reference<const T>::type> 
   operator=(T const& x) const
   {
       return detail::named<Tag, BOOST_DEDUCED_TYPENAME detail::unwrap_cv_reference<const T>::type>(x);
   }
#endif

   template <class T>
   detail::named<Tag, typename detail::unwrap_cv_reference<T>::type> 
   operator=(T& x) const
   {
       return detail::named<Tag, BOOST_DEDUCED_TYPENAME detail::unwrap_cv_reference<T>::type>(x);
   }

   template <class T>
   detail::named<Tag,T> operator()(T& x) const
   {
       return detail::named<Tag,T>(x);
   }
#if !BOOST_WORKAROUND(BOOST_MSVC, == 1200)  // partial ordering bug
   template <class T>
   detail::named<Tag,T const> operator()(T const& x) const
   {
       return detail::named<Tag,T const>(x);
   }
#endif

   template <class Default>
   detail::named_default<Tag, Default>
   operator|(Default& default_) const
   {
       return detail::named_default<Tag, Default>(default_);
   }

#if !BOOST_WORKAROUND(BOOST_MSVC, == 1200)  // partial ordering bug
   template <class Default>
   detail::named_default<Tag, const Default>
   operator|(const Default& default_) const
   {
       return detail::named_default<Tag, const Default>(default_);
   }
#endif 
 
   template <class Default>
   detail::lazy_named_default<Tag, Default>
   operator||(const Default& default_) const
   {
       return detail::lazy_named_default<Tag, Default>(default_);
   }
};

template <class Tag, class HasDefault, class Predicate>
struct named_param
{
    typedef Tag key_type;
    typedef HasDefault has_default;
    typedef Predicate predicate;
};

namespace detail
{
  BOOST_PYTHON_IS_XXX_DEF(named_param,named_param,3)

  template <class T> struct get_key_type { typedef typename T::key_type type; };
  
  template <class T>
  struct key_type
    : mpl::eval_if<
          is_named_param<T>
        , get_key_type<T>
        , mpl::identity<T>
      >
  {
  };

  template <class T> struct get_has_default { typedef  typename T::has_default type; };
  
  template <class T>
  struct has_default
    : mpl::eval_if<
          is_named_param<T>
        , get_has_default<T>
        , mpl::true_
      >
  {
  };
  
  template <class T> struct get_predicate { typedef  typename T::predicate type; };
  
  template <class T>
  struct predicate
    : mpl::eval_if<
          is_named_param<T>
        , get_predicate<T>
        , mpl::identity<mpl::always<mpl::true_> >
      >
  {
  };

  template <class T>
  struct as_arg
  {
      typedef named_param<
          typename key_type<T>::type
        , typename has_default<T>::type
        , typename predicate<T>::type
      > type;
  };

  // labels T with keyword KW if it is not already named
  template <class KW, class T>
  struct as_named
  {
      typedef typename mpl::if_<
          is_named<T>
        , T
        , named<typename key_type<KW>::type, typename unwrap_cv_reference<T const>::type>
      >::type type;
  };

#if BOOST_WORKAROUND(BOOST_MSVC, == 1200)
  template <>
  struct as_named<int,int>
  {
      typedef int type;
  };
#endif

  // Seq ::= a list of named<K,T> objects
  // Arg ::= an arg<...> instantiation
  //
  //    if (named<Arg::key_type,U> in Seq)
  //        return Arg::predicate<U>::type
  //    else
  //        return Arg::has_default
  template <class Seq, class Arg>
  struct keyword_passes_predicate_aux
  {
      BOOST_STATIC_CONSTANT(
          bool, value = (
              sizeof(
                  to_yesno(
                      Seq::keyword_passes_predicate((Arg*)0)
                      ))
              == sizeof(yes_t)
          )
      );

      typedef mpl::bool_<value> type;
  };
  
  template <class Seq, class K>
  struct keyword_passes_predicate
    : keyword_passes_predicate_aux<Seq, typename as_arg<K>::type>
  {};

  template <class B /* = mpl::true_ */>
  struct restrict_keywords
  {
      template<class Keywords>
      struct apply
      {
          struct type_ : Keywords
          {
              type_(const Keywords&) {}
          };
          typedef type_ type;
      };
  };

#if BOOST_WORKAROUND(BOOST_MSVC, == 1200)
  // ETI workaround
  template <>
  struct restrict_keywords<int>
  {
      template <class Keywords>
      struct apply
      {
          typedef int type;
      };
  };
#endif
  
  template <>
  struct restrict_keywords<mpl::false_>
  {
      template<class>
      struct apply
      {};
  };

  // Given actual argument types T0...Tn, return a list of
  // named<U0...Um> types where:
  //
  //   T0...Tm != nil
  //
  // and
  //
  //   Ui = Ti is named<...> ? Ti : named<Ki,Ti>
  //
  template<
      BOOST_PP_ENUM_BINARY_PARAMS(
          BOOST_NAMED_PARAMS_MAX_ARITY, class T, = nil BOOST_PP_INTERCEPT
      )
  >
  struct make_named_list
  {
      template<
          BOOST_PP_ENUM_PARAMS(BOOST_NAMED_PARAMS_MAX_ARITY, class K)
      >
      struct apply
      {
          typedef list<
              typename as_named<K0, T0>::type
            , typename BOOST_PP_CAT(mpl::apply, BOOST_NAMED_PARAMS_MAX_ARITY)<
                  make_named_list<
                      BOOST_PP_ENUM_SHIFTED_PARAMS(
                          BOOST_NAMED_PARAMS_MAX_ARITY, T
                      )
                    , nil
                  >
                , BOOST_PP_ENUM_SHIFTED_PARAMS(BOOST_NAMED_PARAMS_MAX_ARITY, K)
                , nil
              >::type
          > type;
      };
  };

  template <>
  struct make_named_list<
      BOOST_PP_ENUM_PARAMS(BOOST_NAMED_PARAMS_MAX_ARITY, nil BOOST_PP_INTERCEPT)
  >
  {
      template<
          BOOST_PP_ENUM_PARAMS(BOOST_NAMED_PARAMS_MAX_ARITY, class K)
      >
      struct apply
      {
          typedef nil type;
      };
  };
} // namespace detail

#define BOOST_KEYWORDS_TEMPLATE_ARGS(z, n, text) class BOOST_PP_CAT(K, n) = detail::nil

template<
     class K0
   , BOOST_PP_ENUM_SHIFTED(BOOST_NAMED_PARAMS_MAX_ARITY, BOOST_KEYWORDS_TEMPLATE_ARGS, _)
>
struct keywords
{

#undef BOOST_KEYWORDS_TEMPLATE_ARGS

    typedef keywords<
        BOOST_PP_ENUM_PARAMS(BOOST_NAMED_PARAMS_MAX_ARITY, K)
    > self_t;

#ifndef BOOST_NO_SFINAE
    // if the elements of NamedList match the criteria of overload
    // resolution, returns a type which can be constructed from
    // self_t.  Otherwise, this is not a valid metafunction (no nested
    // ::type).

# define BOOST_PASSES_PREDICATE(z, n, text) \
    detail::keyword_passes_predicate<NamedList, BOOST_PP_CAT(K, n)>

    template <class NamedList>
    struct restrict_base
    {
        // metafunction forwarding here would confuse vc6
        typedef mpl::apply1<
            detail::restrict_keywords<
                typename mpl::and_<
                    BOOST_PP_ENUM(BOOST_NAMED_PARAMS_MAX_ARITY, BOOST_PASSES_PREDICATE, _)
                >::type
            >
          , self_t
        > type;
    };

# undef BOOST_PASSES_PREDICATE
    
#endif

    // Instantiations are to be used as an optional argument to control SFINAE
    template<
        BOOST_PP_ENUM_BINARY_PARAMS(
            BOOST_NAMED_PARAMS_MAX_ARITY, class T, = detail::nil BOOST_PP_INTERCEPT
        )       
    >
    struct restrict
#ifndef BOOST_NO_SFINAE
      : restrict_base<
            // Build a list of named<K,T> items for each keyword and actual 
            BOOST_DEDUCED_TYPENAME BOOST_PP_CAT(mpl::apply, BOOST_NAMED_PARAMS_MAX_ARITY)<
                detail::make_named_list<
                    BOOST_PP_ENUM_PARAMS(BOOST_NAMED_PARAMS_MAX_ARITY, T)
                >
              , BOOST_PP_ENUM_PARAMS(BOOST_NAMED_PARAMS_MAX_ARITY, K)
            >::type
        >::type
#endif 
    {
#ifdef BOOST_NO_SFINAE
        typedef self_t type;
#endif 
    };

    detail::nil operator()() const
    {
       return detail::nil();
    }

#define BOOST_PP_ITERATION_PARAMS_1 (3,( \
        1,BOOST_NAMED_PARAMS_MAX_ARITY,<boost/detail/named_params_iterate.hpp> \
    ))
#include BOOST_PP_ITERATE()

};

} // namespace boost

#undef BOOST_NAMED_PARAMS_GCC2

#include <boost/preprocessor/tuple/elem.hpp>
#include <boost/preprocessor/repetition/repeat_from_to.hpp>
#include <boost/preprocessor/arithmetic/inc.hpp>
#include <boost/preprocessor/logical/bool.hpp>
#include <boost/preprocessor/punctuation/comma_if.hpp>
#include <boost/preprocessor/control/expr_if.hpp>

#define BOOST_NAMED_PARAMS_FUN_TEMPLATE_HEAD1(n) \
    template<BOOST_PP_ENUM_PARAMS(n, class T)>
#define BOOST_NAMED_PARAMS_FUN_TEMPLATE_HEAD0(n)

#define BOOST_NAMED_PARAMS_FUN_DECL(z, n, params) \
    BOOST_PP_CAT(BOOST_NAMED_PARAMS_FUN_TEMPLATE_HEAD, BOOST_PP_BOOL(n))(n) \
    BOOST_PP_TUPLE_ELEM(3, 0, params) \
        BOOST_PP_TUPLE_ELEM(3, 1, params)( \
            BOOST_PP_ENUM_BINARY_PARAMS(n, const T, &p) \
            BOOST_PP_COMMA_IF(n) \
            BOOST_PP_EXPR_IF(n, typename) BOOST_PP_TUPLE_ELEM(3, 2, params)::restrict \
            <\
                BOOST_PP_ENUM_PARAMS(n, T)\
            >::type kw = BOOST_PP_TUPLE_ELEM(3, 2, params)()\
        ) \
    { \
        return BOOST_PP_CAT(BOOST_PP_TUPLE_ELEM(3, 1, params), _with_named_params)( \
            kw(BOOST_PP_ENUM_PARAMS(n, p)) \
        ); \
    }

#define BOOST_NAMED_PARAMS_FUN(ret, name, lo, hi, keywords) \
    template<class Params> \
    ret BOOST_PP_CAT(name, _with_named_params)(const Params&);\
    BOOST_PP_REPEAT_FROM_TO(lo, BOOST_PP_INC(hi), BOOST_NAMED_PARAMS_FUN_DECL, (ret, name, keywords)) \
    template<class Params> \
    ret BOOST_PP_CAT(name, _with_named_params)(const Params& p)

#include <boost/iterator/detail/config_undef.hpp>
 
#endif // BOOST_NAMED_PARAMS_031014_HPP

