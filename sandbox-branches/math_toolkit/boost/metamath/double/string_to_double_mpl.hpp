// Copyright (C) 2005 Peder Holt
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_METAMATH_DOUBLE_STRING_TO_DOUBLE_HPP
#define BOOST_METAMATH_DOUBLE_STRING_TO_DOUBLE_HPP

#include <boost/metamath/double/double.hpp>
#include <boost/metamath/double/integral_to_double.hpp>
#include <boost/metamath/double/divides.hpp>
#include <boost/metamath/double/times.hpp>
#include <boost/metamath/double/plus.hpp>
#include <boost/mpl/vector/vector40.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/not.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/plus.hpp>
#include <boost/mpl/begin_end.hpp>
#include <boost/preprocessor/repetition/enum_params_with_a_default.hpp>

namespace boost { namespace metamath {

/*double_ exponent10(int exp)
{
    double_ result;
    return pow(integral_to_double(10),exp);
}*/
    namespace detail {
        typedef integral_c_to_double<long,0>::type zero;
		typedef integral_c_to_double<long,1>::type one;
		typedef integral_c_to_double<long,10>::type ten;

        template<typename Iter>
        struct is_zero {
            BOOST_STATIC_CONSTANT(unsigned char,deref_iter=(mpl::deref<Iter>::type::value));
            BOOST_STATIC_CONSTANT(bool,value=deref_iter==0 || deref_iter=='0');
            typedef mpl::bool_<value> type;
        };
        template<typename Iter>
        struct is_digit {
            BOOST_STATIC_CONSTANT(unsigned char,deref_iter=(mpl::deref<Iter>::type::value));
            BOOST_STATIC_CONSTANT(bool,value= (deref_iter>=0 && deref_iter<=9) || (deref_iter>='0' && deref_iter<='9'));
            typedef mpl::bool_<value> type;
        };
        template<typename Iter>
        struct get_digit {
            BOOST_STATIC_CONSTANT(unsigned char,deref_iter=mpl::deref<Iter>::type::value);            
            BOOST_STATIC_CONSTANT(int,value= (deref_iter>=0 && deref_iter<=9)?deref_iter:((deref_iter>='0' && deref_iter<='9')?deref_iter-'0':0));
            typedef int value_type;
            typedef mpl::int_<value> type;
        };
        template<typename Iter>
        struct is_decimal_mark {
            BOOST_STATIC_CONSTANT(unsigned char,deref_iter=(mpl::deref<Iter>::type::value));
            BOOST_STATIC_CONSTANT(bool,value= (deref_iter=='.'));
            typedef mpl::bool_<value> type;
        };
        template<typename Iter>
        struct is_exponent {
            BOOST_STATIC_CONSTANT(unsigned char,deref_iter=(mpl::deref<Iter>::type::value));
            BOOST_STATIC_CONSTANT(bool,value= (deref_iter=='E' || deref_iter=='e'));
            typedef mpl::bool_<value> type;
        };
        template<typename Iter>
        struct is_minus {
            BOOST_STATIC_CONSTANT(unsigned char,deref_iter=(mpl::deref<Iter>::type::value));
            BOOST_STATIC_CONSTANT(bool,value= (deref_iter=='-'));
            typedef mpl::bool_<value> type;
        };
        template<typename Iter>
        struct is_sign {
            BOOST_STATIC_CONSTANT(unsigned char,deref_iter=(mpl::deref<Iter>::type::value));
            BOOST_STATIC_CONSTANT(bool,value= (deref_iter=='-') || (deref_iter=='+'));
            typedef mpl::bool_<value> type;
        };

        template<typename Iter,typename End,typename Data>
        struct extract_integral_digits;

        template<typename Iter,typename End,typename Data>
        struct next_integral_digits {
            typedef typename plus_double_double<
                typename times_double_double<
                    Data,
                    ten
                >::type,
                integral_to_double<get_digit<Iter> >
            >::type next_data;
            typedef typename extract_integral_digits<typename mpl::next<Iter>::type,End,next_data>::type type;
            typedef typename extract_integral_digits<typename mpl::next<Iter>::type,End,next_data>::iter iter;
        };

        template<typename Iter,typename Data>
        struct finish_sequence {
            typedef Data type;
            typedef Iter iter;
        };

        template<typename Iter,typename End,typename Data>
        struct extract_integral_digits {
            typedef typename mpl::if_<
                //IsDigit
                mpl::and_<mpl::not_<is_same<Iter,End> >,is_digit<Iter> >,
                next_integral_digits<Iter,End,Data>,
                finish_sequence<Iter,Data>
            >::type integral_digits;
            typedef typename integral_digits::type type;
            typedef typename integral_digits::iter iter;
        };
        template<typename Iter,typename End,typename Data,typename Divisor>
        struct extract_fraction_digits_next;

        template<typename Iter,typename End,typename Data,typename Divisor>
        struct next_fraction_digits {
            typedef typename plus_double_double<
                Data,
                typename divides_double_double<
                    integral_to_double<get_digit<Iter> >,
                    Divisor
                >::type
            >::type next_data;
            typedef typename times_double_double<Divisor,ten>::type next_divisor;

            typedef typename extract_fraction_digits_next<Iter,End,next_data,next_divisor>::type type;
            typedef typename extract_fraction_digits_next<Iter,End,next_data,next_divisor>::iter iter;
        };

        template<typename Iter,typename End,typename Data,typename Divisor>
        struct extract_fraction_digits_next
        {
            typedef typename mpl::next<Iter>::type next_iter;
            typedef typename mpl::if_<
                //IsDigit
                mpl::and_<mpl::not_<is_same<next_iter,End> >,is_digit<next_iter> >,
                next_fraction_digits<next_iter,End,Data,Divisor>,
                finish_sequence<next_iter,Data>
            >::type fraction_digits;
            typedef typename fraction_digits::type type;
            typedef typename fraction_digits::iter iter;
        };

        template<typename Iter,typename End,typename Data>
        struct extract_fraction_digits {
            typedef typename mpl::if_<
                //isDecimalMark
                mpl::and_<mpl::not_<is_same<Iter,End> >,is_decimal_mark<Iter> >,
                extract_fraction_digits_next<Iter,End,Data,ten>,
                finish_sequence<Iter,Data>
            >::type fraction_digits;
            typedef typename fraction_digits::type type;
            typedef typename fraction_digits::iter iter;
        };

        template<typename Iter,typename End>
        struct extract_sign {
            typedef typename mpl::if_<
                //IsMinus
                mpl::and_<mpl::not_<is_same<Iter,End> >,is_minus<Iter> >,
                mpl::true_,
                mpl::false_
            >::type type;
            typedef typename mpl::if_<
                mpl::and_<mpl::not_<is_same<Iter,End> >,is_sign<Iter> >,
                typename mpl::next<Iter>::type,
                Iter
            >::type iter;
        };

        template<typename Iter,typename End,typename Data>
        struct extract_integral;

        template<typename Iter,typename End,typename Data>
        struct next_integral {
            typedef typename mpl::plus<
                typename mpl::times<
                    Data,
                    mpl::int_<10>
                >::type,
                typename get_digit<Iter>::type
            >::type next_data;
            typedef typename extract_integral<typename mpl::next<Iter>::type,End,next_data>::type type;
            typedef typename extract_integral<typename mpl::next<Iter>::type,End,next_data>::iter iter;
        };

        template<typename Iter,typename End,typename Data>
        struct extract_integral {
            typedef typename mpl::if_<
                //IsDigit
                mpl::and_<mpl::not_<is_same<Iter,End> >,is_digit<Iter> >,
                next_integral<Iter,End,Data>,
                finish_sequence<Iter,Data>
            >::type integral_digits;
            typedef typename integral_digits::type type;
            typedef typename integral_digits::iter iter;
        };
        template<typename Iter,typename End,typename Data,typename Divisor>
        struct extract_fraction_digits_next;

        template<typename Double,short Exponent>
        struct exponent_pow {
	        typedef typename mpl::if_<
		        mpl::bool_<Exponent&1>,
		        Double,
		        one
	        >::type type0;
	        typedef typename times_double_double<
		        type0,
		        typename exponent_pow<
			        typename mpl::times<Double,Double>::type,
			        (Exponent>>1)
		        >::type 
	        >::type type;
        };

        template<typename N1>
        struct exponent_pow<N1,0> {
	        typedef one type;
        };

        template<typename Iter,typename End,typename Data>
        struct extract_exponent_next
        {
            typedef typename mpl::next<Iter>::type next_iter;
            typedef typename extract_sign<next_iter,End>::type sign;
            typedef typename extract_sign<next_iter,End>::iter iter;
            typedef typename extract_integral<iter,End,mpl::int_<0> >::type number;
            typedef typename exponent_pow<ten,number::value>::type exponent_pow;
            typedef typename mpl::eval_if<
                sign,
                divides_double_double<Data,exponent_pow>,
                times_double_double<Data,exponent_pow>
            >::type type;
        };

        template<typename Iter,typename End,typename Data>
        struct extract_exponent {
            typedef typename mpl::eval_if<
                //isDecimalMark
                mpl::and_<mpl::not_<is_same<Iter,End> >,is_exponent<Iter> >,
                extract_exponent_next<Iter,End,Data>,
                finish_sequence<Iter,Data>
            >::type type;
        };
    } //namespace detail

    template<typename Container>
    struct string_to_double {
        typedef typename mpl::begin<Container>::type begin_iter;
        typedef typename mpl::end<Container>::type end_iter;
        //Extract sign
        typedef detail::extract_sign<begin_iter,end_iter> extract_sign;
        typedef typename extract_sign::type sign;
        typedef typename extract_sign::iter sign_iter;
        //Extract integral digits
        typedef detail::extract_integral_digits<sign_iter,end_iter,detail::zero > extract_integral_digits;
        typedef typename extract_integral_digits::type integral_digits;
        typedef typename extract_integral_digits::iter integral_digits_iter;
        //Extract fraction digits
        typedef detail::extract_fraction_digits<integral_digits_iter,end_iter,integral_digits> extract_fraction_digits;
        typedef typename extract_fraction_digits::type fraction_digits;
        typedef typename extract_fraction_digits::iter fraction_digits_iter;
        //Extract exponent
        typedef typename detail::extract_exponent<fraction_digits_iter,end_iter,fraction_digits>::type number;
        typedef typename detail::round<double_<typename number::mantissa,number::exponent,sign::value> >::type type;
    };
#define BOOST_METAMATH_MACRO(z,n,data) mpl::int_<BOOST_PP_CAT(D,n)>

    template<BOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT(40,unsigned char D,255)>
    struct string_c_to_double : string_to_double<mpl::vector40<BOOST_PP_ENUM(40,BOOST_METAMATH_MACRO,~)> >::type
    {
    };

#undef BOOST_METAMATH_MACRO
}} //namespace boost::metamath

#endif

/*
double_ string_c_to_double(BOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT(40,unsigned char a,255))
{
    double_ arg;
    std::vector<char> chars;
    BOOST_PP_REPEAT(40,BOOOST_METAMATH_PUSH_BACK,~);
    std::vector<char>::iterator it=chars.begin();
    if(*it=='-') {
        arg.m_sign=true;
        ++it;
    }
    while((*it=='0' || *it==0) && it!=chars.end()) ++it;

    //Extract the integral part
    std::vector<char>::iterator start_integer=it;
    int integer_size=0;
    while(it!=chars.end() && ((*it>=0 && *it<=9) || (*it>='0' && *it<='9'))) {
        ++it;
        ++integer_size;
    }
    double_ result;
    std::vector<char>::iterator end_integer=it;
    unsigned long integer=0;
    if(integer_size>0) {
        for(std::vector<char>::iterator it=start_integer;it!=end_integer;++it) {
            char value;
            if(*it>=0 && *it<=9) value=*it;
            else if(*it>='0' && *it<='9') value=*it-'0';
            integer=integer*10+value;
            if(integer>=100000000) {
                result=result+integral_to_double(integer)*exponent10(integer_size-9);
                integer_size-=9;
                integer=0;
            }
        }
        if(integer!=0) result=result+integral_to_double(integer);
    }
    //Extract the fraction part
    if(it!=chars.end() && *it=='.') {
        ++it;
        std::vector<char>::iterator start_fraction=it;
        int fraction_exponent=0;
        while(it!=chars.end() && (*it==0 || *it=='0')) {
            ++it;
            --fraction_exponent;
        }
        int fraction=0;
        while(it!=chars.end() && ((*it>=0 && *it<=9) || (*it>='0' && *it<='9'))) {
            char value;
            if(*it>=0 && *it<=9) value=*it;
            else if(*it>='0' && *it<='9') value=*it-'0';
            fraction=fraction*10+value;
            --fraction_exponent;
            if(fraction>=100000000) {
                result=result+integral_to_double(fraction)*exponent10(fraction_exponent);
                fraction=0;
            }
            ++it;
        }
        if(fraction!=0) {
            result=result+integral_to_double(fraction)*exponent10(fraction_exponent);
        }
    }
    //Extract exponent
    if(it!=chars.end() && (*it=='E' || *it=='e')) {
        short exponent=0;
        short sign=1;
        ++it;
        if(*it=='-') {
            sign=-1;
            ++it;
        }
        while(it!=chars.end() && ((*it>=0 && *it<=9) || (*it>='0' && *it<='9'))) {
            ++it;
            char value;
            if(*it>=0 && *it<=9) value=*it;
            else if(*it>='0' && *it<='9') value=*it-'0';
            exponent=exponent*10+value;
        }
        result=result*exponent10(exponent*sign);
    }
    return result;
}*/