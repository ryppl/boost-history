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
#include <boost/mpl/apply_if.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/is_convertible.hpp>
#include <boost/iterator/detail/config_def.hpp>
#include <boost/python/detail/is_xxx.hpp>
//#include <boost/mpl/aux_/has_xxx.hpp>

namespace boost {

template<class T>
struct keyword;

template <class Tag, typename has_default_ = mpl::true_, class Predicate = mpl::always<mpl::true_> >
struct arg;

namespace detail
{ 
  // just to avoid confusion.  We have to build a conversion operator
  // that returns this type to get around ETI; int seemed a little too
  // dangerous.
  struct non_int_eti_type { private: non_int_eti_type(); };
  
  typedef char yes_t;
  struct no_t { char x[128]; };

  yes_t to_yesno(mpl::true_);
  no_t to_yesno(mpl::false_);

  
  template<class KW, class Default>
  struct named_default
  {
      named_default(Default& x)
        : default_(x)
      {}
      
      Default& default_;
  };

  struct nil
  {
#if BOOST_WORKAROUND(BOOST_MSVC, <= 1300)
      template<class KW>
      struct apply
      {
         typedef nil type;
      };

      template<class KW, class Default>
      struct apply_value
      {
         typedef Default type;
      };

      template<class K, class Default>
      Default& get(const named_default<K, Default>& x) const
      {
          return x.default_;
      }
#else
      template<class K, class Default>
      Default& operator[](const named_default<K, Default>& x) const
      {
          return x.default_;
      }
#endif
      // No keyword was found if we get here, so we should only return
      // mpl::true_ if it's OK to have a default for the argument.
      template <class Arg>
      static typename Arg::has_default keyword_passes_predicate(Arg*);

      typedef mpl::true_ has_default;
      typedef mpl::always<mpl::true_> predicate;
  };

  // A tuple of labeled argument holders
  // Restructured this so that head isn't inherited
  // We'll need a version without using declarations for vc6/7.0
  template <class H, class T = nil>
  struct list : T
  {
#if BOOST_WORKAROUND(BOOST_MSVC, <= 1300)
     typedef list<H, T> self_t;

     template<class KW>
     struct apply
       : mpl::apply_if<
             boost::is_same<KW, typename H::key_type>
           , mpl::identity<self_t>
           , typename T::template apply<KW>
         >
     {};

     template<class KW, class Default>
     struct apply_value
        : mpl::apply_if<
              boost::is_same<KW, typename H::key_type>
            , mpl::identity<typename H::value_type>
            , typename T::template apply_value<KW, Default>
          >
      {};
#endif
      H head;

      list(H h, T t) : T(t), head(h) {}

#if BOOST_WORKAROUND(BOOST_MSVC, <= 1300)
      template<class KW>
      typename apply_value<KW, nil>::type&
      operator[](const keyword<KW>& x) const
      {
         typedef typename apply<KW>::type list_t;
         return static_cast<const list_t&>(*this).get(x);
      }

      template<class KW, class Default>
      typename apply_value<KW, Default>::type&
      operator[](const named_default<KW, Default>& x) const
      {
         typedef typename apply<KW>::type list_t;
         return static_cast<const list_t&>(*this).get(x);
      }

      typename H::value_type& get(const keyword<typename H::key_type>& x) const
      {
          return head[x];
      }

      template<class Default>
      typename H::value_type& get(const named_default<typename H::key_type, Default>& x) const
      {
          return head[x];
      }
#else
      typename H::value_type& operator[](const keyword<typename H::key_type>& x) const
      {
          return head[x];
      }

      template<class Default>
      typename H::value_type& operator[](const named_default<typename H::key_type, Default>& x) const
      {
          return head[x];
      }
		
		using T::operator[];

      template <class HasDefault, class Predicate>
      static typename mpl::apply1<
          typename mpl::lambda<Predicate>::type
        , typename H::value_type
      >::type
      keyword_passes_predicate(arg<typename H::key_type,HasDefault,Predicate>*);

      using T::keyword_passes_predicate;
#endif
  };

//  struct named_base {};

  template <class KW, class T>
  struct named // : named_base
  {
      typedef KW key_type;
      typedef T value_type;

      named(T& x) : val(x) {}
      
      T& operator[](const keyword<KW>&) const
      {
          return val;
      }

      template<class Default>
      T& operator[](const named_default<KW, Default>& x) const
      {
          return val;
      }

      T& val;
  };

  BOOST_PYTHON_IS_XXX_DEF(named,named,2)
} // namespace detail

template <class Tag>
struct keyword
{
#if !BOOST_WORKAROUND(BOOST_MSVC, == 1200)  // partial ordering bug
   template <class T>
   detail::named<Tag,T const> operator=(T const& x) const
   {
      return detail::named<Tag,T const>(x);
   }
#endif
	
	template <class T>
   detail::named<Tag,T> operator=(T& x) const
   {
      return detail::named<Tag,T>(x);
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
    
   template<class Default>
   detail::named_default<Tag, Default>
   operator|(Default& default_) const
   {
      return detail::named_default<Tag, Default>(default_);
   }
   
#if !BOOST_WORKAROUND(BOOST_MSVC, == 1200)  // partial ordering bug
   template<class Default>
   detail::named_default<Tag, const Default>
   operator|(const Default& default_) const
   {
      return detail::named_default<Tag, const Default>(default_);
   }
#endif 
};

template <class Tag, typename has_default_, class Predicate>
struct arg
{
    typedef Tag key_type;
    typedef has_default_ has_default;
    typedef Predicate predicate;
};

namespace detail
{
  BOOST_PYTHON_IS_XXX_DEF(arg,arg,3)

  template <class T> struct get_key_type { typedef typename T::key_type type; };
  
  template <class T>
  struct key_type
    : mpl::apply_if<
          is_arg<T>
        , get_key_type<T>
        , mpl::identity<T>
      >
  {
  };

  template <class T> struct get_has_default { typedef  typename T::has_default type; };
  
  template <class T>
  struct has_default
    : mpl::apply_if<
          is_arg<T>
        , get_has_default<T>
        , mpl::true_
      >
  {
  };
  
  template <class T> struct get_predicate { typedef  typename T::predicate type; };
  
  template <class T>
  struct predicate
    : mpl::apply_if<
          is_arg<T>
        , get_predicate<T>
        , mpl::identity<mpl::always<mpl::true_> >
      >
  {
  };

  template <class T>
  struct as_arg
  {
      typedef arg<
          typename key_type<T>::type
        , typename has_default<T>::type
        , typename predicate<T>::type
      > type;
  };

  // labels T with keyword KW if it is not already named
  template<class KW, class T>
  struct as_named
  {
      typedef typename mpl::if_<
          is_named<T>
//          is_convertible<T*,named_base*>
        , T
        , named<typename key_type<KW>::type,T const>
      >::type type;
  };

#if BOOST_WORKAROUND(BOOST_MSVC, == 1200)
  template<>
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
  template<class Seq, class Arg>
  struct keyword_passes_predicate_aux
  {
      typedef typename mpl::lambda<typename Arg::predicate>::type
      pred_expr;

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
  
  template<class Seq, class K>
  struct keyword_passes_predicate
    : keyword_passes_predicate_aux<Seq, typename as_arg<K>::type>
  {};

  template<class B /* = mpl::true_ */>
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
  
  template<>
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
      class T0
    , class T1
    , class T2
    , class T3
    , class T4
  >
  struct make_named_list
  {
      template<
          class K0
        , class K1
        , class K2
        , class K3
        , class K4
      >
      struct apply
      {
          typedef list<
              typename as_named<K0, T0>::type
            , typename mpl::apply5<
                  make_named_list<T1, T2, T3, T4, nil>
                , K1, K2, K3, K4, nil
              >::type
          > type;
      };
  };

  template<>
  struct make_named_list<nil,nil,nil,nil,nil>
  {
      template<
          class K0
        , class K1
        , class K2
        , class K3
        , class K4
      >
      struct apply
      {
          typedef nil type;
      };
  };
} // namespace detail

template<
     class K0
   , class K1 = detail::nil
   , class K2 = detail::nil
   , class K3 = detail::nil
   , class K4 = detail::nil
>
struct keywords
{
    typedef keywords<K0,K1,K2,K3,K4> self_t;

#ifndef BOOST_NO_SFINAE
    // if the elements of NamedList match the criteria of overload
    // resolution, returns a type which can be constructed from
    // self_t.  Otherwise, this is not a valid metafunction (no nested
    // ::type).
    template<class NamedList>
    struct restrict_base
    {
        // metafunction forwarding here would confuse vc6
        typedef mpl::apply1<
            detail::restrict_keywords<
                typename mpl::and_<
                    detail::keyword_passes_predicate<NamedList, K0>
                  , detail::keyword_passes_predicate<NamedList, K1>
                  , detail::keyword_passes_predicate<NamedList, K2>
                  , detail::keyword_passes_predicate<NamedList, K3>
                  , detail::keyword_passes_predicate<NamedList, K4>
                >::type
            >
          , self_t
        > type;
    };

# if BOOST_WORKAROUND(BOOST_MSVC, == 1200)
    // To satisfy ETI
    operator detail::non_int_eti_type() const;
# endif 

# if  BOOST_WORKAROUND(BOOST_MSVC, <= 1300)
    // ETI workaround
    template <>
    struct restrict_base<int>
    {
        typedef detail::non_int_eti_type type;
    };
# endif
#endif

    // Instantiations are to be used as an optional argument to control SFINAE
    template<
        class T0 // These are actual argument types
      , class T1 = detail::nil
      , class T2 = detail::nil
      , class T3 = detail::nil
      , class T4 = detail::nil
    >
    struct restrict
#ifndef BOOST_NO_SFINAE
      : restrict_base<
            // Build a list of named<K,T> items for each keyword and actual 
            BOOST_DEDUCED_TYPENAME mpl::apply5<
                detail::make_named_list<T0,T1,T2,T3,T4>
              , K0
              , K1
              , K2
              , K3
              , K4
            >::type
        >::type
#endif 
    {
#ifdef BOOST_NO_SFINAE
        typedef self_t type;
#endif 
    };

    template<class A0>
    detail::list<
        BOOST_DEDUCED_TYPENAME detail::as_named<K0, A0>::type
      , detail::nil
    >
    operator()(const A0& a0) const
    {
        // for cwpro8
        typedef typename detail::as_named<K0, A0>::type t0;
        
        t0 n0(a0);

        typedef detail::list<t0> l0;
        
        return l0(n0, detail::nil());
    }

    template<class A0, class A1>
    detail::list<
        BOOST_DEDUCED_TYPENAME detail::as_named<K0, A0>::type
      , detail::list<
            BOOST_DEDUCED_TYPENAME detail::as_named<K1, A1>::type
          , detail::nil
        >
    >
    operator()(const A0& a0, const A1& a1) const
    {
        // for cwpro8
        typedef typename detail::as_named<K0, A0>::type t0;
        typedef typename detail::as_named<K1, A1>::type t1;
        
        t0 n0(a0);
        t1 n1(a1);

        typedef detail::list<t1> l1;
        typedef detail::list<t0,l1> l0;
        
        return l0(n0, l1(n1, detail::nil()));
    }

    template<class A0, class A1, class A2>
    detail::list<
        BOOST_DEDUCED_TYPENAME detail::as_named<K0, A0>::type
      , detail::list<
            BOOST_DEDUCED_TYPENAME detail::as_named<K1, A1>::type
          , detail::list<
                BOOST_DEDUCED_TYPENAME detail::as_named<K2, A2>::type
              , detail::nil
            >
        >
    >
    operator()(const A0& a0, const A1& a1, const A2& a2) const
    {
        // for cwpro8
        typedef typename detail::as_named<K0, A0>::type t0;
        typedef typename detail::as_named<K1, A1>::type t1;
        typedef typename detail::as_named<K2, A2>::type t2;
        
        t0 n0(a0);
        t1 n1(a1);
        t2 n2(a2);

        typedef detail::list<t2> l2;
        typedef detail::list<t1,l2> l1;
        typedef detail::list<t0,l1> l0;
        
        return l0(n0, l1(n1, l2(n2, detail::nil())));
    }

    template<class A0, class A1, class A2, class A3>
    detail::list<
        typename detail::as_named<K0, A0>::type
      , detail::list<
            typename detail::as_named<K1, A1>::type
          , detail::list<
                typename detail::as_named<K2, A2>::type
              , detail::list<
                    typename detail::as_named<K3, A3>::type
                >
            >
        >
    >
    operator()(const A0& a0, const A1& a1, const A2& a2, const A3& a3) const
    {
        // for cwpro8
        typedef typename detail::as_named<K0, A0>::type t0;
        typedef typename detail::as_named<K1, A1>::type t1;
        typedef typename detail::as_named<K2, A2>::type t2;
        typedef typename detail::as_named<K3, A3>::type t3;
        
        t0 n0(a0);
        t1 n1(a1);
        t2 n2(a2);
        t3 n3(a3);

        typedef detail::list<t3> l3;
        typedef detail::list<t2,l3> l2;
        typedef detail::list<t1,l2> l1;
        typedef detail::list<t0,l1> l0;
        
        return l0(n0, l1(n1, l2(n2, l3(n3, detail::nil()))));
    }
};

} // namespace boost

#endif // BOOST_NAMED_PARAMS_031014_HPP

