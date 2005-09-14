// Copyright (C) 2005 Peder Holt
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_METAMATH_DOUBLE_POWER_HPP
#define BOOST_METAMATH_DOUBLE_POWER_HPP

#include <boost/metamath/double.hpp>
#include <boost/metamath/double/times.hpp>
#include <boost/metamath/double/constants.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/eval_if.hpp>

namespace boost { namespace metamath {
    namespace detail {
        template<typename N1,typename T,T N2>
        struct power_impl_double_int {
            BOOST_STATIC_CONSTANT(bool,isZero=(N2==0));
            BOOST_STATIC_CONSTANT(bool,negative=(N2<0));
            BOOST_STATIC_CONSTANT(T,value2=negative?N2*-1:N2);
	        typedef typename mpl::if_<
		        mpl::bool_<value2&1>,
		        N1,
		        meta::one
	        >::type type0;

            typedef typename mpl::eval_if<
                mpl::bool_<isZero>,
                meta::one,
                power_impl_double_int<
                    times_double_double<N1,N1>,
                    T,
                    (value2>>1)
                >
            >::type type1;

            typedef times_double_double<type0,type1> type2;

            typedef typename mpl::eval_if<
                mpl::bool_<negative>,
                divides_double_double<meta::one,type2>,
                type2
            >::type type;
        };

        template<typename N1,typename N2>
        struct power_double_int : power_impl_double_int<N1,typename N2::value_type,N2::value>::type
        {};

        template<int N1,int N2>
        struct power_impl_int_int {
	        BOOST_STATIC_CONSTANT(int,value0=(N2&1?N1:1)); 
	        BOOST_STATIC_CONSTANT(int,value=(value0*power_impl_int_int<N1*N1,(N2>>1)>::value));
	        typedef mpl::int_<value> type;
        };

        template<int N1>
        struct power_impl_int_int<N1,0> {	
	        BOOST_STATIC_CONSTANT(int,value=1); 
	        typedef mpl::int_<value> type;
        };

        template<int N2>
        struct power_impl_int_int<-1,N2> {	
	        BOOST_STATIC_CONSTANT(int,value=N2&1?-1:1); 
	        typedef mpl::int_<value> type;
        };
        template<>
        struct power_impl_int_int<-1,0> {	
	        BOOST_STATIC_CONSTANT(int,value=1); 
	        typedef mpl::int_<value> type;
        };
    }//namespace detail

    template<typename Tag1,typename Tag2>
    struct power_impl;

    template<>
    struct power_impl<double_c_tag,mpl::integral_c_tag>
    {
        template<typename N1,typename N2> struct apply : detail::power_double_int<N1,N2>
	    {
	    };
    };

    template<>
    struct power_impl<mpl::integral_c_tag,mpl::integral_c_tag>
    {
        template<typename N1,typename N2> struct apply : detail::power_impl_int_int<N1::value,N2::value>::type
	    {
	    };
    };

}}  //namespace boost::metamath

#endif
