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
#include <boost/mpl/not.hpp>
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

template<class T> class reference_wrapper;

namespace named_params {

// Forward declaration for aux::arg_list, below.
template<class T> struct keyword;

namespace aux
{
  // types used with the "sizeof trick" to capture the results of
  // overload resolution at compile-time.
  typedef char yes_t;
  typedef char (&no_t)[2];

  // mpl::true_ and mpl::false_ are not distinguishable by sizeof(),
  // so we pass them through these functions to get a type that is.
  yes_t to_yesno(mpl::true_);
  no_t to_yesno(mpl::false_);


  // A wrapper for the default value passed by the user when resolving
  // the value of the parameter with the given Keyword
  template <class Keyword, class Value>
  struct default_
  {
      default_(Value& x)
        : value(x)
      {}

      Value& value;
  };

  //
  // lazy_default -- 
  //
  //    A wrapper for the default value computation function passed by
  //    the user when resolving the value of the parameter with the
  //    given keyword
  //
#if BOOST_WORKAROUND(__EDG_VERSION__, <= 300)
  // These compilers need a little extra help with overload
  // resolution; we have empty_arg_list's operator[] accept a base
  // class to make that overload less preferable.
  template <class KW, class DefaultComputer>
  struct lazy_default_base
  {
      lazy_default_base(DefaultComputer const& x)
        : default_(x)
      {}
      DefaultComputer const& compute_default;
  };

  template <class KW, class DefaultComputer>
  struct lazy_default
    : lazy_default_base<KW,DefaultComputer>
  {
      lazy_default(DefaultComputer const & x)
        : lazy_default_base<KW,DefaultComputer>(x)
      {}
  };
# define BOOST_NAMED_PARAMS_lazy_default_fallback lazy_default_base
#else 
  template <class KW, class DefaultComputer>
  struct lazy_default
  {
      lazy_default(const DefaultComputer& x)
        : compute_default(x)
      {}
      DefaultComputer const& compute_default;
  };
# define BOOST_NAMED_PARAMS_lazy_default_fallback lazy_default
#endif 

  //
  // Structures used to build the tuple of actual arguments.  The
  // tuple is a nested cons-style list of arg_list specializations
  // terminated by an empty_arg_list.
  //
  // Each specialization of arg_list is derived from its successor in
  // the list type.  This feature is used along with using
  // declarations to build member function overload sets that can
  // match against keywords.
  //
  
  // Terminates arg_list<> and represents an empty list.  Since this
  // is just the terminating case you might want to look at arg_list
  // first, to get a feel for what's really happening here.
  struct empty_arg_list
  {
      empty_arg_list() {}

      // Constructor taking BOOST_NAMED_PARAMS_MAX_ARITY empty_arg_list
      // arguments; this makes initialization
      empty_arg_list(
          BOOST_PP_ENUM_PARAMS(
              BOOST_NAMED_PARAMS_MAX_ARITY, empty_arg_list BOOST_PP_INTERCEPT
      ))
      {}

#if BOOST_WORKAROUND(BOOST_MSVC, <= 1300) || BOOST_NAMED_PARAMS_GCC2
      // The overload set technique doesn't work with these older
      // compilers, so they need some explicit handholding.
      
      // A metafunction class that, given a keyword, returns the base
      // sublist whose get() function can produce the value for that
      // key.
      struct key_owner
      {
          template<class KW>
          struct apply
          {
              typedef empty_arg_list type;
          };
      };

      // A metafunction class that, given a keyword and a default
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

      template <class K, class T>
      T& get(default_<K,T> x) const
      {
          return x.value;
      }

      template <class K, class F>
      typename F::result_type get(lazy_default<K,F> x) const
      {
          return x.compute_default();
      }
#endif

      // If this function is called, it means there is no argument
      // in the list that matches the supplied keyword. Just return
      // the default value.
      template <class K, class Default>
      Default& operator[](default_<K, Default> x) const
      {
          return x.value;
      }

      // If this function is called, it means there is no argument
      // in the list that matches the supplied keyword. Just evaluate
      // and return the default value.
      template <class K, class F>
      typename F::result_type operator[](
          BOOST_NAMED_PARAMS_lazy_default_fallback<K,F> x) const
      {
          return x.compute_default();
      }
#undef BOOST_NAMED_PARAMS_lazy_default_fallback

      // No keyword was found if we get here, so we should only return
      // mpl::true_ if it's OK to have a default for the argument.
      template <class Arg>
      static typename Arg::has_default keyword_passes_predicate(Arg*);
  };

  // Used to pass static information about parameter requirements
  // through the keyword_passes_predicate() overload set (below).  The
  // matched function is never invoked, but its type indicates whether
  // a parameter matches at compile-time
  template <class Keyword, class Predicate, class HasDefault>
  struct parameter_requirements
  {
      typedef HasDefault has_default;
  };

  template <class KW, class T>
  struct arg_holder;

  // A tuple of labeled argument holders, terminated with empty_arg_list.
  // Every ArgHolder is an instance of arg_holder<>.
  template <class ArgHolder, class Next = empty_arg_list>
  struct arg_list : Next
  {
#if BOOST_WORKAROUND(BOOST_MSVC, <= 1300) || BOOST_NAMED_PARAMS_GCC2
      // A metafunction class which, given a keyword, returns the base
      // sublist whose get() function can produce the value for that
      // key.
      struct key_owner
      {
          template<class KW>
          struct apply
            : mpl::eval_if<
                  boost::is_same<KW, typename ArgHolder::key_type>
                , mpl::identity<arg_list<ArgHolder,Next> >
                , mpl::apply_wrap1<typename Next::key_owner,KW>
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
                  boost::is_same<KW, typename ArgHolder::key_type>
                , mpl::identity<typename ArgHolder::value_type>
                , mpl::apply_wrap2<typename Next::key_value_type, KW, Default>
              >
          {
          };
      };
#endif

      ArgHolder holder;

      template<
          BOOST_PP_ENUM_PARAMS(BOOST_NAMED_PARAMS_MAX_ARITY, class A)
      >
      arg_list(
          BOOST_PP_ENUM_BINARY_PARAMS(BOOST_NAMED_PARAMS_MAX_ARITY, A, const & a)
      )
        : Next(
              BOOST_PP_ENUM_SHIFTED_PARAMS(BOOST_NAMED_PARAMS_MAX_ARITY, a)
            , empty_arg_list()
          )
        , holder(a0)
      {}

      arg_list(ArgHolder arg, Next const& next)
        : Next(next)
        , holder(arg)
      {}

#if BOOST_WORKAROUND(BOOST_MSVC, <= 1300) || BOOST_NAMED_PARAMS_GCC2
      template <class KW>
      typename mpl::apply_wrap2<key_value_type, KW, empty_arg_list>::type&
      operator[](keyword<KW> x) const
      {
          typename mpl::apply_wrap1<key_owner, KW>::type const& sublist = *this;
          return sublist.get(x);
      }

      template <class KW, class Default>
      typename mpl::apply_wrap2<key_value_type, KW, Default>::type&
      operator[](default_<KW, Default> x) const
      {
          typename mpl::apply_wrap1<key_owner, KW>::type const& sublist = *this;
          return sublist.get(x);
      }

      template <class KW, class F>
      typename mpl::apply_wrap2<
          key_value_type,KW
        , typename F::result_type
      >::type
      operator[](lazy_default<KW,F> x) const
      {
          typename mpl::apply_wrap1<key_owner, KW>::type const& sublist = *this;
          return sublist.get(x);
      }

      typename ArgHolder::value_type& get(keyword<typename ArgHolder::key_type> x) const
      {
          return holder[x];
      }

      template <class Default>
      typename ArgHolder::value_type& 
      get(default_<typename ArgHolder::key_type, Default> x) const
      {
          return holder[x];
      }

      template <class Default>
      typename ArgHolder::value_type& 
      get(lazy_default<typename ArgHolder::key_type, Default> x) const
      {
          return holder[x];
      }
#else
      typename ArgHolder::value_type&
      operator[](keyword<typename ArgHolder::key_type> x) const
      {
          return holder[x];
      }

      template <class Default>
      typename ArgHolder::value_type&
      operator[](default_<typename ArgHolder::key_type, Default> x) const
      {
          return holder[x];
      }

      template <class Default>
      typename ArgHolder::value_type&
      operator[](lazy_default<typename ArgHolder::key_type, Default> x) const
      {
          return holder[x];
      }

      using Next::operator[];

      template <class HasDefault, class Predicate>
      static typename mpl::apply1<
          Predicate
        , typename ArgHolder::value_type
      >::type
      keyword_passes_predicate(
          parameter_requirements<typename ArgHolder::key_type,Predicate,HasDefault>*
      );

      using Next::keyword_passes_predicate;
#endif

      // Comma operator to compose argument list without using keywords<>.
      // Useful for argument lists with undetermined length.
      template <class KW, class T2>
      arg_list<arg_holder<KW, T2>, arg_list> 
      operator,(arg_holder<KW, T2> const& x) const
      {
          return arg_list<arg_holder<KW, T2>, arg_list>(x, *this);
      }
  };

  template <> struct arg_list<int,int> {};

  // Holds a name-tagged argument reference.
  template <class KW, class T>
  struct arg_holder
  {
      typedef KW key_type;
      typedef T value_type;

      arg_holder(T& x) : val(x) {}

      T& operator[](keyword<KW>) const
      {
          return val;
      }

      template <class Default>
      T& operator[](default_<KW, Default>) const
      {
          return val;
      }

      template <class Default>
      T& operator[](lazy_default<KW, Default>) const
      {
          return val;
      }

      // Comma operator to compose argument list without using keywords<>.
      // Useful for argument lists with undetermined length.
      template <class KW2, class T2>
      arg_list<
          arg_holder<KW, T>
        , arg_list<arg_holder<KW2, T2> > 
      >
      operator,(arg_holder<KW2, T2> const& x) const
      {
          return arg_list<
              arg_holder<KW, T>
            , arg_list<arg_holder<KW2, T2> > 
          >(
              *this
            , arg_list<arg_holder<KW2, T2> >(x, empty_arg_list())
          );
      }

      T& val;
  };

  BOOST_PYTHON_IS_XXX_DEF(arg_holder,arg_holder,2)

  template <class U>
  yes_t is_cv_reference_wrapper_check(reference_wrapper<U> const volatile*);
  no_t is_cv_reference_wrapper_check(...);

  // Returns mpl::true_ if T is of type reference_wrapper<U> cv
  template <class T>
  struct is_cv_reference_wrapper
  {
     BOOST_STATIC_CONSTANT(
         bool, value = (
             sizeof(is_cv_reference_wrapper_check((T*)0)) == sizeof(yes_t)
         )
     );

     typedef mpl::bool_<value> type;
  };

  // Produces the unwrapped type to hold a reference to in named<>
  // Can't use boost::unwrap_reference<> here because it
  // doesn't handle the case where T = reference_wrapper<U> cv
  template <class T>
  struct unwrap_cv_reference
  {
      typedef typename mpl::eval_if<
          is_cv_reference_wrapper<T>
        , T
        , mpl::identity<T>
      >::type type;
  };

} // namespace aux

template <class Tag>
struct keyword
{
#if !BOOST_WORKAROUND(BOOST_MSVC, == 1200)  // partial ordering bug
    template <class T>
    aux::arg_holder<
        Tag
      , typename aux::unwrap_cv_reference<T const>::type
    > 
    operator=(T const& x) const
    {
        return aux::arg_holder<
            Tag
          , BOOST_DEDUCED_TYPENAME aux::unwrap_cv_reference<T const>::type
        >(x);
    }
#endif

    template <class T>
    aux::arg_holder<
        Tag
      , typename aux::unwrap_cv_reference<T>::type
    > 
    operator=(T& x) const
    {
        return aux::arg_holder<
            Tag
          , BOOST_DEDUCED_TYPENAME aux::unwrap_cv_reference<T>::type
        >(x);
    }

    template <class Default>
    aux::default_<Tag, Default>
    operator|(Default& default_) const
    {
        return aux::default_<Tag, Default>(default_);
    }

#if !BOOST_WORKAROUND(BOOST_MSVC, == 1200)  // partial ordering bug
    template <class Default>
    aux::default_<Tag, const Default>
    operator|(const Default& default_) const
    {
        return aux::default_<Tag, const Default>(default_);
    }
#endif 

    template <class Default>
    aux::lazy_default<Tag, Default>
    operator||(Default const& default_) const
    {
        return aux::lazy_default<Tag, Default>(default_);
    }
};

// These templates can be used to describe the treatment of particular
// named parameters for the purposes of overload elimination with
// SFINAE, by placing specializations in the keywords<...> list.  In
// order for a treated function to participate in overload resolution:
//
//   - all keyword tags wrapped in required<...> must have a matching
//     actual argument
//
//   - The actual argument type matched by every keyword tag
//     associated with a predicate must satisfy that predicate
//
// If a keyword k is specified without an optional<...> or
// required<...>, wrapper, it is treated as though optional<k> were
// specified.
//
template <class Tag, class Predicate = mpl::always<mpl::true_> >
struct required
{
    typedef Tag key_type;
    typedef Predicate predicate;
};

template <class Tag, class Predicate = mpl::always<mpl::true_> >
struct optional
{
    typedef Tag key_type;
    typedef Predicate predicate;
};

namespace aux
{
  BOOST_PYTHON_IS_XXX_DEF(required, required, 2)
  BOOST_PYTHON_IS_XXX_DEF(optional, optional, 2)

  template <class T> struct get_key_type { typedef typename T::key_type type; };

  template <class T>
  struct key_type
    : mpl::eval_if<
          mpl::or_<
              is_optional<T>
            , is_required<T>
          >
        , get_key_type<T>
        , mpl::identity<T>
      >
  {
  };

  template <class T>
  struct has_default
    : mpl::not_<typename is_required<T>::type>
  {
  };

  template <class T> struct get_predicate { typedef typename T::predicate type; };

  template <class T>
  struct predicate
    : mpl::eval_if<
         mpl::or_<
              is_optional<T>
            , is_required<T>
          >
        , get_predicate<T>
        , mpl::identity<mpl::always<mpl::true_> >
      >
  {
  };

  template <class T>
  struct as_parameter_requirements
  {
      typedef parameter_requirements<
          typename key_type<T>::type
        , typename predicate<T>::type
        , typename has_default<T>::type
      > type;
  };

  // labels T with keyword KW if it is not already named
  template <class KW, class T>
  struct as_arg_holder
  {
      typedef typename mpl::if_<
          is_arg_holder<T>
        , T
        , arg_holder<
              typename key_type<KW>::type
            , typename unwrap_cv_reference<T const>::type
          >
      >::type type;
  };

#if BOOST_WORKAROUND(BOOST_MSVC, == 1200)
  template <>
  struct as_arg_holder<int,int>
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
                  aux::to_yesno(
                      Seq::keyword_passes_predicate((Arg*)0)
                  )
              ) == sizeof(yes_t)
          )
      );

      typedef mpl::bool_<value> type;
  };

  template <class Seq, class K>
  struct keyword_passes_predicate
    : keyword_passes_predicate_aux<Seq, typename as_parameter_requirements<K>::type>
  {};

  template <class Seq>
  struct keyword_passes_predicate<Seq,void>
    : mpl::true_
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
  // arg_holder<U0...Um> types where:
  //
  //   T0...Tm != empty_arg_list
  //
  // and
  //
  //   Ui = Ti is arg_holder<...> ? Ti : arg_holder<Ki,Ti>
  //
  template<
      BOOST_PP_ENUM_BINARY_PARAMS(
          BOOST_NAMED_PARAMS_MAX_ARITY, class T, = empty_arg_list BOOST_PP_INTERCEPT
      )
  >
  struct make_arg_list
  {
      template<
          BOOST_PP_ENUM_PARAMS(BOOST_NAMED_PARAMS_MAX_ARITY, class K)
      >
      struct apply
      {
          typedef arg_list<
              typename as_arg_holder<K0, T0>::type
            , typename BOOST_PP_CAT(mpl::apply_wrap, BOOST_NAMED_PARAMS_MAX_ARITY)<
                  make_arg_list<
                      BOOST_PP_ENUM_SHIFTED_PARAMS(
                          BOOST_NAMED_PARAMS_MAX_ARITY, T
                      )
                    , empty_arg_list
                  >
                , BOOST_PP_ENUM_SHIFTED_PARAMS(BOOST_NAMED_PARAMS_MAX_ARITY, K)
                , empty_arg_list
              >::type
          > type;
      };
  };

  template <>
  struct make_arg_list<
      BOOST_PP_ENUM_PARAMS(BOOST_NAMED_PARAMS_MAX_ARITY, empty_arg_list BOOST_PP_INTERCEPT)
  >
  {
      template<
          BOOST_PP_ENUM_PARAMS(BOOST_NAMED_PARAMS_MAX_ARITY, class K)
      >
      struct apply
      {
          typedef empty_arg_list type;
      };
  };

} // namespace aux

#define BOOST_KEYWORDS_TEMPLATE_ARGS(z, n, text) class BOOST_PP_CAT(K, n) = void

template<
     class K0
   , BOOST_PP_ENUM_SHIFTED(BOOST_NAMED_PARAMS_MAX_ARITY, BOOST_KEYWORDS_TEMPLATE_ARGS, _)
>
struct keywords
{

#undef BOOST_KEYWORDS_TEMPLATE_ARGS

#ifndef BOOST_NO_SFINAE
    // if the elements of NamedList match the criteria of overload
    // resolution, returns a type which can be constructed from
    // keywords.  Otherwise, this is not a valid metafunction (no nested
    // ::type).

# define BOOST_PASSES_PREDICATE(z, n, text) \
    aux::keyword_passes_predicate<NamedList, BOOST_PP_CAT(K, n)>

    template <class NamedList>
    struct restrict_base
    {
        // metafunction forwarding here would confuse vc6
        typedef mpl::apply1<
            aux::restrict_keywords<
                typename mpl::and_<
                    BOOST_PP_ENUM(BOOST_NAMED_PARAMS_MAX_ARITY, BOOST_PASSES_PREDICATE, _)
                >::type
            >
          , keywords

        > type;
    };

# undef BOOST_PASSES_PREDICATE
    
#endif

    // Instantiations are to be used as an optional argument to control SFINAE
    template<
        BOOST_PP_ENUM_BINARY_PARAMS(
            BOOST_NAMED_PARAMS_MAX_ARITY, class T, = aux::empty_arg_list BOOST_PP_INTERCEPT
        )       
    >
    struct restrict
#ifndef BOOST_NO_SFINAE
      : restrict_base<
            // Build a list of named_arg_holder<K,T> items for each keyword and actual 
            BOOST_DEDUCED_TYPENAME BOOST_PP_CAT(mpl::apply, BOOST_NAMED_PARAMS_MAX_ARITY)<
                aux::make_arg_list<
                    BOOST_PP_ENUM_PARAMS(BOOST_NAMED_PARAMS_MAX_ARITY, T)
                >
              , BOOST_PP_ENUM_PARAMS(BOOST_NAMED_PARAMS_MAX_ARITY, K)
            >::type
        >::type
#endif 
    {
#ifdef BOOST_NO_SFINAE
        typedef keywords type;
#endif 
    };

    aux::empty_arg_list operator()() const
    {
       return aux::empty_arg_list();
    }

#define BOOST_PP_ITERATION_PARAMS_1 (3,( \
        1,BOOST_NAMED_PARAMS_MAX_ARITY,<boost/detail/named_params_iterate.hpp> \
    ))
#include BOOST_PP_ITERATE()

};

} // namespace named_params

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

