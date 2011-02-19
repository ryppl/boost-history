//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_REF_CONVERT_TRAITS_CHECKING_ER_2010_HPP
#define BOOST_ASSIGN_V2_REF_CONVERT_TRAITS_CHECKING_ER_2010_HPP
#include <boost/mpl/assert.hpp>
//#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/assign/v2/ref/convert_traits/reference.hpp>

namespace boost{
namespace assign{ 
namespace v2{
namespace ref{
namespace convert_traits{
namespace checking{

    template<typename T1, typename T2, typename T3>
    struct tester{
    
        typedef typename convert_traits::reference<T1, T2>::type result_;
                
        static void test()
        {
            BOOST_MPL_ASSERT((boost::is_same<result_,T3>));
        }

   };

namespace same_value_type{

    template<typename T>
    void do_check()
    {

        tester<const T &, const T &, const T &>::test(); 
        tester<const T &,       T &, const T &>::test(); 
        tester<      T &, const T &, const T &>::test(); 
        tester<      T &,       T &,       T &>::test(); 

        tester<const T &, const T  , const T  >::test(); 
        tester<const T &,       T  , const T  >::test(); 
        tester<      T &, const T  , const T  >::test(); 
        tester<      T &,       T  ,       T  >::test(); 

        tester<const T ,  const T &, const T  >::test(); 
        tester<const T ,        T &, const T  >::test(); 
        tester<      T ,  const T &, const T  >::test(); 
        tester<      T ,        T &,       T  >::test(); 

        tester<const T ,  const T  , const T  >::test(); 
        tester<const T ,        T  , const T  >::test(); 
        tester<      T ,  const T  , const T  >::test(); 
        tester<      T ,        T  ,       T  >::test(); 

    }

}// twin_values
// TODO upgrade
/*
namespace distinct_value_type{

    template<typename T1,typename T2,typename T3>
    void do_check_impl(const T1&, const T2& ,const T3&) 
    {
        
        tester<const T1&, const T2&, const T3 >::test(); 
        tester<const T1&,       T2&, const T3 >::test(); 
        tester<      T1&, const T2&, const T3 >::test(); 
        tester<      T1&,       T2&,       T3 >::test(); 

        tester<const T1&, const T2 , const T3 >::test(); 
        tester<const T1&,       T2 , const T3 >::test(); 
        tester<      T1&, const T2 , const T3 >::test(); 
        tester<      T1&,       T2 ,       T3 >::test(); 

        tester<const T1 ,  const T2&, const T3 >::test(); 
        tester<const T1 ,        T2&, const T3 >::test(); 
        tester<      T1 ,  const T2&, const T3 >::test(); 
        tester<      T1 ,        T2&,       T3 >::test(); 

        tester<const T1 ,  const T2 , const T3 >::test(); 
        tester<const T1 ,        T2 , const T3 >::test(); 
           tester<      T1 ,  const T2 , const T3 >::test(); 
        tester<      T1 ,        T2 ,       T3 >::test(); 

    }

#define BOOST_ASSIGN_V2_MACRO(T1, T2, T3) \
    template<typename T> \
    void do_check( \
        typename boost::enable_if< \
            boost::is_same<T,T1> \
        >::type* = 0 \
    ) \
    { \
        distinct_values::do_check_impl( T1(), T2(), T3() ); \
    } \
*/
/**/
/*
// don't try to guess the supertype, it may vary by compiler. 
namespace supertype_{
    typedef boost::numeric::conversion_traits<short, int>::supertype a_;
    typedef boost::numeric::conversion_traits<int, long>::supertype b_;
    typedef boost::numeric::conversion_traits<float, double>::supertype c_;
    typedef boost::numeric::conversion_traits<
        double,long double>::supertype d_;
}// supertype_
BOOST_ASSIGN_V2_MACRO(short  , int,     supertype_::a_)
BOOST_ASSIGN_V2_MACRO(int    , long,    supertype_::b_)
BOOST_ASSIGN_V2_MACRO(float  , double,    supertype_::c_)
typedef long double long_double_;
BOOST_ASSIGN_V2_MACRO(double , long_double_, workaround::d_)
#undef BOOST_ASSIGN_V2_MACRO
}// distinct_value_type
*/

}// checking
}// convert_traits
}// ref
}// v2
}// assign
}// boost

#endif
