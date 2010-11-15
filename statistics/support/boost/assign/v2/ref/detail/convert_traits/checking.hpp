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
#include <boost/mpl/apply.hpp>
#include <boost/mpl/not.hpp>
#include <boost/config.hpp>
#include <boost/static_assert.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/promote.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/assign/v2/ref/detail/convert_traits/result.hpp>

namespace boost{
namespace assign{ 
namespace v2{
namespace ref{
namespace convert_traits{
namespace checking{

    template<typename T1,typename T2,typename T3>
    struct tester{
    
        typedef typename convert_traits::result<T1,T2>::type result_;
                
        static void test()
        {
            typedef boost::is_same<result_,T3> pred_;
            BOOST_STATIC_ASSERT( pred_::value );
        }

   };

namespace twin_values{

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

    };

}// twin_values
namespace distinct_values{
namespace arithmetic{

    template<typename T1,typename T2,typename T3>
    void do_check_impl(const T1&, const T2& ,const T3&) 
    {
        
        // Disable for Ubuntu 9.1 - GCC 4.4 (see workaround below).
        // typedef typename boost::mpl::not_<
        //   boost::is_same<T1,T2> 
        // >::type not_same_;
        // BOOST_STATIC_ASSERT(not_same_::value);
        
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

    };


#define m(T1, T2, T3)													    \
    template<typename T>													\
    void do_check(															\
    	typename boost::enable_if<											\
    		boost::is_same<T,T1> 											\
        >::type* = 0														\
    ) 																		\
    {																		\
        distinct_values::arithmetic::do_check_impl( T1(), T2(), T3() );	    \
    }																		\
/**/

// Works for Max OSX 10.6 GCC 4.2 but not Ubuntu 9.1 GCC 4.4
// 	m(short  , long,	int)
// 	m(int    , long,	long)
//	m(float  , double,	double)
//  typedef long double long_double_;
//	m(double , long_double_, long double)

namespace workaround{
	typedef boost::numeric::conversion_traits<short, int>::supertype a_;
	typedef boost::numeric::conversion_traits<int, long>::supertype b_;
	typedef boost::numeric::conversion_traits<float, double>::supertype c_;
	typedef boost::numeric::conversion_traits<
    	double,long double>::supertype d_;
}// workaround

m(short  , int, 	workaround::a_)
m(int    , long,	workaround::b_)
m(float  , double,	workaround::c_)
typedef long double long_double_;
m(double , long_double_, workaround::d_)

#undef m
}// arithmetic
}// distinct_values
}// checking
}// convert_traits
}// ref
}// v2
}// assign
}// boost

#endif
