
// Copyright (c) Aleksey Gurtovoy 2000-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//

// Preprocessed version of "boost/mpl/apply.hpp" header
// -- DO NOT modify by hand!

namespace boost { namespace mpl {

template<
      typename F
    >
struct apply0
    : apply_wrap0<
          typename lambda<F>::type
       
        >
{
    enum { arity = 1 }; typedef F arg1;
 friend class apply0_rebind;
 typedef apply0_rebind rebind;
 };
 class apply0_rebind { public: template< typename U1 > struct apply : apply0<U1> { };
 

};

template<>
struct apply0<int>
{
    typedef int type;
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
    enum { arity = 2 }; typedef F arg1;
 typedef T1 arg2;
 friend class apply1_rebind;
 typedef apply1_rebind rebind;
 };
 class apply1_rebind { public: template< typename U1, typename U2 > struct apply : apply1< U1,U2 > { };
 

};

template<>
struct apply1< int,int >
{
    typedef int type;
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
    enum { arity = 3 }; typedef F arg1;
 typedef T1 arg2;
 typedef T2 arg3;
 friend class apply2_rebind;
 typedef apply2_rebind rebind;
 };
 class apply2_rebind { public: template< typename U1, typename U2, typename U3 > struct apply : apply2< U1,U2,U3 > { };
 

};

template<>
struct apply2< int,int,int >
{
    typedef int type;
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
    enum { arity = 4 }; typedef F arg1;
 typedef T1 arg2;
 typedef T2 arg3;
 typedef T3 arg4;
 friend class apply3_rebind;
 typedef apply3_rebind rebind;
 };
 class apply3_rebind { public: template< typename U1, typename U2, typename U3, typename U4 > struct apply : apply3< U1,U2,U3,U4 > { };
 

};

template<>
struct apply3< int,int,int,int >
{
    typedef int type;
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
    enum { arity = 5 }; typedef F arg1;
 typedef T1 arg2;
 typedef T2 arg3;
 typedef T3 arg4;
 typedef T4 arg5;
 friend class apply4_rebind;
 typedef apply4_rebind rebind;
 };
 class apply4_rebind { public: template< typename U1, typename U2, typename U3, typename U4, typename U5 > struct apply : apply4< U1,U2,U3,U4,U5 > { };
 

};

template<>
struct apply4< int,int,int,int,int >
{
    typedef int type;
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
    enum { arity = 6 }; typedef F arg1;
 typedef T1 arg2;
 typedef T2 arg3;
 typedef T3 arg4;
 typedef T4 arg5;
 typedef T5 arg6;
 friend class apply5_rebind;
 typedef apply5_rebind rebind;
 };
 class apply5_rebind { public: template< typename U1, typename U2, typename U3, typename U4, typename U5, typename U6 > struct apply : apply5< U1,U2,U3,U4,U5,U6 > { };
 

};

template<>
struct apply5< int,int,int,int,int,int >
{
    typedef int type;
};

}}

