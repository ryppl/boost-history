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

namespace boost {

template<class T>
struct keyword;

namespace detail
{

  // just to avoid confusion.  We have to build a conversion operator
  // that returns this type to get around ETI; int seemed a little too
  // dangerous.
  struct non_int_eti_type { private: non_int_eti_type(); };
  
   template<class KW, class Default>
   struct get_keyword
   {
      get_keyword(Default& x)
         : default_(x)
      {}
      
      Default& default_;
   };

   struct nil
   {
//      typedef nil value_type;
  //    typedef void key_type;

      template<class K, class Default>
      Default& operator[](const get_keyword<K, Default>& x) const
      {
         return x.default_;
      }

      typedef mpl::true_ has_default;
      typedef mpl::always<mpl::true_> predicate_type;
   };

   // A tuple of labeled argument holders
   template <class H, class T>
   struct list : H, T
   {
      list(H h, T t) : H(h), T(t) {}

#ifdef BOOST_MSVC // yes, even 7.1 has this bug
      typename H::value_type& operator[](const keyword<typename H::key_type>& x) const
      {
         return static_cast<const H&>(*this)[x];
      }

      template<class Default>
      typename H::value_type& operator[](const get_keyword<typename H::key_type, Default>& x) const
      {
         return static_cast<const H&>(*this)[x];
      }
      
      using T::operator[];
#else
      using T::operator[];
      using H::operator[];
#endif 
   };

   struct named_base {};

   template <class KW, class T>
   struct named : named_base
   {
      typedef KW key_type;
      typedef T value_type;

      named(T& x) : val(x) {}
      
      template<class Default>
      T& operator[](const get_keyword<KW, Default>& x) const
      {
         return val;
      }

      T& operator[](const keyword<KW>&) const
      {
         return val;
      }

      T& val;
   };

} // namespace detail
 
template <class Derived>
struct keyword
{
   typedef mpl::true_ has_default;
   typedef mpl::always<mpl::true_> predicate_type;

   template <class T>
   detail::named<Derived,T> operator=(T& x) const
   {
      return detail::named<Derived,T>(x);
   }

   template <class T>
   detail::named<Derived,T const> operator=(T const& x) const
   {
      return detail::named<Derived,T const>(x);
   }

   template <class T>
   detail::named<Derived,T> operator()(T& x) const
   {
      return detail::named<Derived,T>(x);
   }
#if !BOOST_WORKAROUND(BOOST_MSVC, == 1200)  // partial ordering bug
   template <class T>
   detail::named<Derived,T const> operator()(T const& x) const
   {
      return detail::named<Derived,T const>(x);
   }
#endif
    
   template<class Default>
   detail::get_keyword<Derived, Default>
   operator|(Default& default_) const
   {
      return detail::get_keyword<Derived, Default>(default_);
   }
   
   template<class Default>
   detail::get_keyword<Derived, const Default>
   operator|(const Default& default_) const
   {
      return detail::get_keyword<Derived, const Default>(default_);
   }
};

namespace detail
{

   typedef char yes_t;
   struct no_t { char x[128]; };

   yes_t is_named(detail::named_base*);
   no_t is_named(...);

   template<bool Named /*= true */>
   struct as_named_base
   {
      template<class KW, class T>
      struct apply
      {
         typedef T type;
      };
   };

   template<>
   struct as_named_base<false>
   {
      template<class KW, class T>
      struct apply
      {
         typedef detail::named<
                KW
              , const T
         > type;
      };
   };

  template<class KW, class T>
  struct as_named
  {
      // metafunction forwarding would confuse vc6
      BOOST_STATIC_CONSTANT(
          bool, named
                = sizeof(::boost::detail::is_named((T*)0)) == sizeof(yes_t)
      );
      
      typedef typename as_named_base<named>
          ::template apply<KW, T>::type type;
  };

#if BOOST_WORKAROUND(BOOST_MSVC, == 1200)
   template<>
   struct as_named<int,int>
   {
       typedef int type;
   };
#endif 
  
   template<class Seq, class KW>
   struct has_named_of
   {
      typedef typename mpl::lambda<typename KW::predicate_type>::type
         pred_expr;

      static yes_t to_yesno(mpl::true_);
      static no_t to_yesno(mpl::false_);

      template<class T>
      static typename mpl::apply1<pred_expr, T>::type
      check(detail::named<KW, T>*);

      static typename KW::has_default check(void*);

      BOOST_STATIC_CONSTANT(bool,
            value = (sizeof(to_yesno(check((Seq*)0))) == sizeof(yes_t)));

      typedef mpl::bool_<value> type;
   };

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

   template<
        class T0
      , class T1
      , class T2
      , class T3
      , class T4
   >
   struct make_as_named_list
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
                 make_as_named_list<T1, T2, T3, T4, nil>
               , K1, K2, K3, K4, nil
             >::type
         > type;
      };
   };

   template<>
   struct make_as_named_list<nil,nil,nil,nil,nil>
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

   template<class Seq>
   struct restrict_base
   {
       // metafunction forwarding here would confuse vc6
       typedef typename mpl::apply1<
           detail::restrict_keywords<
               typename mpl::and_<
                   detail::has_named_of<Seq, K0>
                 , detail::has_named_of<Seq, K1>
                 , detail::has_named_of<Seq, K2>
                 , detail::has_named_of<Seq, K3>
                 , detail::has_named_of<Seq, K4>
               >::type
           >
         , self_t
       >::type type;
   };

#if BOOST_WORKAROUND(BOOST_MSVC, == 1200)
    // To satisfy ETI
    operator detail::non_int_eti_type() const;
#endif 

#if  BOOST_WORKAROUND(BOOST_MSVC, <= 1300)
    // ETI workaround
    template <>
    struct restrict_base<int>
    {
        typedef detail::non_int_eti_type type;
    };
#endif

   template<
        class T0
      , class T1 = detail::nil
      , class T2 = detail::nil
      , class T3 = detail::nil
      , class T4 = detail::nil
   >
   struct restrict
   {
       // metafunction forwarding here would confuse vc6
       typedef BOOST_DEDUCED_TYPENAME restrict_base<
           BOOST_DEDUCED_TYPENAME mpl::apply5<
               detail::make_as_named_list<T0,T1,T2,T3,T4>
             , K0, K1, K2, K3, K4
           >::type
       >::type type;
   };

   template<class A0>
   detail::list<
        BOOST_DEDUCED_TYPENAME detail::as_named<K0, A0>::type
      , detail::nil
   >
   operator()(const A0& a0) const
   {
      return detail::list<
           BOOST_DEDUCED_TYPENAME detail::as_named<K0, A0>::type
         , detail::nil>(a0, detail::nil());
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
      return detail::list<
           BOOST_DEDUCED_TYPENAME detail::as_named<K0, A0>::type
         , detail::list<
                BOOST_DEDUCED_TYPENAME detail::as_named<K1, A1>::type
              , detail::nil
           >
      >(a0, detail::list<
                BOOST_DEDUCED_TYPENAME detail::as_named<K1, A1>::type
              , detail::nil
           >(a1, detail::nil()));
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
      return detail::list<
              BOOST_DEDUCED_TYPENAME detail::as_named<K0, A0>::type
            , detail::list<
                 BOOST_DEDUCED_TYPENAME detail::as_named<K1, A1>::type
               , detail::list<
                    BOOST_DEDUCED_TYPENAME detail::as_named<K2, A2>::type
                  , detail::nil
               >
            >
      >(a0, detail::list<
                BOOST_DEDUCED_TYPENAME detail::as_named<K1, A1>::type
              , detail::list<
                    BOOST_DEDUCED_TYPENAME detail::as_named<K2, A2>::type
                  , detail::nil
               >
          >(a1, detail::list<
                    BOOST_DEDUCED_TYPENAME detail::as_named<K2, A2>::type
                  , detail::nil
               >(a2, detail::nil())));
   }
};

} // namespace boost

#endif // BOOST_NAMED_PARAMS_031014_HPP

