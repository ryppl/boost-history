
// Copyright Aleksey Gurtovoy 2000-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//

// Preprocessed version of "boost/mpl/apply.hpp" header
// -- DO NOT modify by hand!

namespace boost { namespace mpl { namespace aux {
struct has_rebind_tag;

template< typename T > struct has_rebind;
}}}

namespace boost { namespace mpl {

template<
      typename F
    >
struct apply0
    : apply_wrap0<
          typename lambda<F>::type
       
        >
{
    static int const arity = 1; typedef F arg1;
 friend class apply0_rebind;
 typedef apply0_rebind rebind;
 };
 template< typename T1 > char operator|( ::boost::mpl::aux::has_rebind_tag, ::boost::mpl::aux::has_rebind< apply0<T1> >* );
 class apply0_rebind { public: template< typename U1 > struct apply : apply0<U1> { };
 

};

template<
      typename F
    >
struct apply< F,na,na,na,na,na >
    : apply0<F>
{
};

template<
      typename F, typename T1
    >
struct apply1
    : apply_wrap1<
          typename lambda<F>::type
        , T1
        >
{
    static int const arity = 2; typedef F arg1;
 typedef T1 arg2;
 friend class apply1_rebind;
 typedef apply1_rebind rebind;
 };
 template< typename T1, typename T2 > char operator|( ::boost::mpl::aux::has_rebind_tag, ::boost::mpl::aux::has_rebind< apply1< T1,T2 > >* );
 class apply1_rebind { public: template< typename U1, typename U2 > struct apply : apply1< U1,U2 > { };
 

};

template<
      typename F, typename T1
    >
struct apply< F,T1,na,na,na,na >
    : apply1< F,T1 >
{
};

template<
      typename F, typename T1, typename T2
    >
struct apply2
    : apply_wrap2<
          typename lambda<F>::type
        , T1, T2
        >
{
    static int const arity = 3; typedef F arg1;
 typedef T1 arg2;
 typedef T2 arg3;
 friend class apply2_rebind;
 typedef apply2_rebind rebind;
 };
 template< typename T1, typename T2, typename T3 > char operator|( ::boost::mpl::aux::has_rebind_tag, ::boost::mpl::aux::has_rebind< apply2< T1,T2,T3 > >* );
 class apply2_rebind { public: template< typename U1, typename U2, typename U3 > struct apply : apply2< U1,U2,U3 > { };
 

};

template<
      typename F, typename T1, typename T2
    >
struct apply< F,T1,T2,na,na,na >
    : apply2< F,T1,T2 >
{
};

template<
      typename F, typename T1, typename T2, typename T3
    >
struct apply3
    : apply_wrap3<
          typename lambda<F>::type
        , T1, T2, T3
        >
{
    static int const arity = 4; typedef F arg1;
 typedef T1 arg2;
 typedef T2 arg3;
 typedef T3 arg4;
 friend class apply3_rebind;
 typedef apply3_rebind rebind;
 };
 template< typename T1, typename T2, typename T3, typename T4 > char operator|( ::boost::mpl::aux::has_rebind_tag, ::boost::mpl::aux::has_rebind< apply3< T1,T2,T3,T4 > >* );
 class apply3_rebind { public: template< typename U1, typename U2, typename U3, typename U4 > struct apply : apply3< U1,U2,U3,U4 > { };
 

};

template<
      typename F, typename T1, typename T2, typename T3
    >
struct apply< F,T1,T2,T3,na,na >
    : apply3< F,T1,T2,T3 >
{
};

template<
      typename F, typename T1, typename T2, typename T3, typename T4
    >
struct apply4
    : apply_wrap4<
          typename lambda<F>::type
        , T1, T2, T3, T4
        >
{
    static int const arity = 5; typedef F arg1;
 typedef T1 arg2;
 typedef T2 arg3;
 typedef T3 arg4;
 typedef T4 arg5;
 friend class apply4_rebind;
 typedef apply4_rebind rebind;
 };
 template< typename T1, typename T2, typename T3, typename T4, typename T5 > char operator|( ::boost::mpl::aux::has_rebind_tag, ::boost::mpl::aux::has_rebind< apply4< T1,T2,T3,T4,T5 > >* );
 class apply4_rebind { public: template< typename U1, typename U2, typename U3, typename U4, typename U5 > struct apply : apply4< U1,U2,U3,U4,U5 > { };
 

};

template<
      typename F, typename T1, typename T2, typename T3, typename T4
    >
struct apply< F,T1,T2,T3,T4,na >
    : apply4< F,T1,T2,T3,T4 >
{
};

template<
      typename F, typename T1, typename T2, typename T3, typename T4
    , typename T5
    >
struct apply5
    : apply_wrap5<
          typename lambda<F>::type
        , T1, T2, T3, T4, T5
        >
{
    static int const arity = 6; typedef F arg1;
 typedef T1 arg2;
 typedef T2 arg3;
 typedef T3 arg4;
 typedef T4 arg5;
 typedef T5 arg6;
 friend class apply5_rebind;
 typedef apply5_rebind rebind;
 };
 template< typename T1, typename T2, typename T3, typename T4, typename T5, typename T6 > char operator|( ::boost::mpl::aux::has_rebind_tag, ::boost::mpl::aux::has_rebind< apply5< T1,T2,T3,T4,T5,T6 > >* );
 class apply5_rebind { public: template< typename U1, typename U2, typename U3, typename U4, typename U5, typename U6 > struct apply : apply5< U1,U2,U3,U4,U5,U6 > { };
 

};

template<
      typename F, typename T1, typename T2, typename T3, typename T4
    , typename T5
    >
struct apply
    : apply5< F,T1,T2,T3,T4,T5 >
{
};

}}

