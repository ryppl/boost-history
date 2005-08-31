// Copyright (C) 2005 Peder Holt
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_METAMATH_DOUBLE_POWER_HPP
#define BOOST_METAMATH_DOUBLE_POWER_HPP

#include <boost/metamath/double.hpp>
#include <boost/metamath/double/times.hpp>
#include <boost/metamath/double/constants.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/eval_if.hpp>

namespace boost { namespace metamath {
    namespace aux_ {
        template<typename N1,int N2>
        struct power_impl_double_int {
	        typedef typename mpl::if_<
		        mpl::bool_<N2&1>,
		        N1,
		        meta::one
	        >::type type0;
	        typedef typename mpl::times<
		        type0,
		        typename power_impl_double_int<
			        typename mpl::times<N1,N1>::type,
			        (N2>>1)
		        >::type 
	        >::type type;
        };

        template<typename N1>
        struct power_impl_double_int<N1,0> {
	        typedef meta::one type;
        };

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
    }//namespace aux_

    template<typename Tag1,typename Tag2>
    struct power_impl;

    template<>
    struct power_impl<double_c_tag,mpl::integral_c_tag>
    {
        template<typename N1,typename N2> struct apply : aux_::power_impl_double_int<N1,N2::value>::type
	    {
	    };
    };

    template<>
    struct power_impl<mpl::integral_c_tag,mpl::integral_c_tag>
    {
        template<typename N1,typename N2> struct apply : aux_::power_impl_int_int<N1::value,N2::value>::type
	    {
	    };
    };

}}  //namespace boost::metamath

#endif
