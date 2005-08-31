// Copyright (C) 2005 Peder Holt
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (http://www.boost.org/LICENSE_1_0.txt)

#if !BOOST_PP_IS_ITERATING

    #ifndef BOOST_METAMATH_DOUBLE_STRING_TO_DOUBLE
    #define BOOST_METAMATH_DOUBLE_STRING_TO_DOUBLE

    #include <boost/metamath/double/double.hpp>
    #include <boost/metamath/double/detail/shift.hpp>
    #include <boost/metamath/double/times.hpp>
    #include <boost/metamath/double/divides.hpp>
    #include <boost/preprocessor/iteration/iterate.hpp>
    #include <boost/preprocessor/repetition/enum_params_with_a_default.hpp>
    #include <boost/mpl/apply.hpp>

#define BOOST_METAMATH_IS_DIGIT(value) ((value>=0 && value<=9) || (value>='0' && value<='9'))
#define BOOST_METAMATH_IS_SIGN(value) (value=='+' || value=='-')
#define BOOST_METAMATH_IS_EXPONENT(value) (value=='e' || value=='E')
#define BOOST_METAMATH_IS_DECIMAL_MARK(value) (value=='.')
#define BOOST_METAMATH_IS_END(value) (value==255)
#define BOOST_METAMATH_GET_DIGIT(value) \
    ((value>=0 && value<=9) ? value:\
    ((value>='0' && value<='9') ? value-'0':0))

namespace boost { namespace metamath {
    namespace detail {
        template<boost::uint16_t Exponent>
        struct pow_impl {
            template<typename T>
            struct apply {
	            typedef typename mpl::if_<
		            mpl::bool_<Exponent&1>,
		            T,
                    typename integral_c_to_double<int,1>::type
	            >::type type0;
	            typedef typename times_double_double<
		            type0,
                    typename mpl::apply1<
                        pow_impl<(Exponent>>1)>,
			            typename times_double_double<T,T>::type
		            >::type 
	            >::type type;
            };            
        };
        template<>
        struct pow_impl<0> {
            template<typename T>
            struct apply {
                typedef typename integral_c_to_double<int,1>::type type;
            };            
        };
        template<typename T,boost::int16_t Exponent>
        struct pow {
            BOOST_STATIC_CONSTANT(bool,sign=Exponent<0);
            BOOST_STATIC_CONSTANT(boost::uint16_t,unsigned_exponent=sign?-Exponent:Exponent);
            typedef typename pow_impl<unsigned_exponent>::apply<T>::type positive_pow;
            typedef typename mpl::eval_if<
                mpl::bool_<sign>,
                divides_double_double<typename integral_c_to_double<int,1>::type,positive_pow>,
                times_double_double<typename integral_c_to_double<int,1>::type,positive_pow>
            >::type type;
        };

        template<boost::int16_t Exponent>
        struct exponent10 : pow<typename integral_c_to_double<int,10>::type,Exponent>::type 
        {};

    }
template<BOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT(40,unsigned char a,255)>
struct string_c_to_double
{
    BOOST_STATIC_CONSTANT(bool,sign=(a0=='-'?true:false));
    enum {
        NumberSign=0x1,
        DecimalMark=0x2,
        ExponentSign=0x4,
        Exponent=0x8,
        ExponentNumber=0x10,
        IntegralNumber=0x20,
        FractionNumber=0x40,
        Overflow=0x80,
        CompleteNumber=0x100,
        NaN=0x200
    };
    BOOST_STATIC_CONSTANT(boost::int16_t,status_0=
    (
        BOOST_METAMATH_IS_SIGN(a0)?
        NumberSign:
        (
            BOOST_METAMATH_IS_DECIMAL_MARK(a0)?
            DecimalMark:
            (
                BOOST_METAMATH_IS_DIGIT(a0)?
                IntegralNumber:
                NaN
            )
        )
    ));
    BOOST_STATIC_CONSTANT(boost::uint32_t,result_a_0=0);
    BOOST_STATIC_CONSTANT(boost::uint32_t,result_b_0=(
        (status_0==NumberSign || status_0==DecimalMark)?
        0:
        (
            (status_0==IntegralNumber)?
            BOOST_METAMATH_GET_DIGIT(a0):
            0 //NaN value
        )
    ));
    BOOST_STATIC_CONSTANT(boost::int16_t,exp10_overflow_0=0);
    BOOST_STATIC_CONSTANT(boost::int16_t,exp10_0=0);

    #define BOOST_PP_ITERATION_PARAMS_1 (3, (1, 39, <boost/metamath/double/string_to_double.hpp>))
    #include BOOST_PP_ITERATE()

    BOOST_STATIC_CONSTANT(boost::int16_t,exp1=(detail::integral_exp<boost::uint32_t,result_a_39>::value));
    BOOST_STATIC_CONSTANT(boost::int16_t,exp2=(detail::integral_exp<boost::uint32_t,result_b_39&0x7fffffff>::value));
    BOOST_STATIC_CONSTANT(boost::int16_t,exp_shift=
        (
            (exp1==0 && exp2==0)?-1023:
            (exp1>0)?(exp1-30):
            (exp2-61)
        )
    );
    typedef detail::shift_mantissa<detail::mantissa<result_a_39,result_b_39&0x7fffffff>,-exp_shift> normalized_mantissa;
    typedef double_<normalized_mantissa,61+exp_shift,sign> result_no_exponent;
    typedef typename detail::round<
        typename detail::times_double_double<result_no_exponent,detail::exponent10<exp10_overflow_39+exp10_39> >::type
    >::type type;
};
}} //namespace boost::metamath
#undef BOOST_METAMATH_IS_DIGIT
#undef BOOST_METAMATH_IS_SIGN
#undef BOOST_METAMATH_IS_EXPONENT
#undef BOOST_METAMATH_IS_DECIMAL_MARK
#undef BOOST_METAMATH_IS_END
#undef BOOST_METAMATH_GET_DIGIT

    #endif
#else

#define N BOOST_PP_ITERATION()
#define N_1 BOOST_PP_DEC(N)

    BOOST_STATIC_CONSTANT(boost::int16_t,BOOST_PP_CAT(status_,N)=(
        (BOOST_METAMATH_IS_DECIMAL_MARK(BOOST_PP_CAT(a,N)) && ((BOOST_PP_CAT(status_,N_1)&NumberSign) || (BOOST_PP_CAT(status_,N_1)&IntegralNumber)))?
        DecimalMark:
        (
            BOOST_METAMATH_IS_DIGIT(BOOST_PP_CAT(a,N))?
            (
                ((BOOST_PP_CAT(status_,N_1)&FractionNumber) || (BOOST_PP_CAT(status_,N_1)&IntegralNumber))?
                (BOOST_PP_CAT(status_,N_1)|((BOOST_PP_CAT(result_a_,N_1)>=100000000)?Overflow:0)):
                (
                    (BOOST_PP_CAT(status_,N_1)&ExponentNumber)?
                    BOOST_PP_CAT(status_,N_1):
                    (
                        (BOOST_PP_CAT(status_,N_1)&NumberSign)?
                        IntegralNumber:
                        (
                            ((BOOST_PP_CAT(status_,N_1)&ExponentSign) || (BOOST_PP_CAT(status_,N_1)&Exponent))?
                            ExponentNumber:
                            (
                                (BOOST_PP_CAT(status_,N_1)&DecimalMark)?
                                (
                                    (BOOST_PP_CAT(result_a_,N_1)==0&&BOOST_PP_CAT(result_b_,N_1)==0)?
                                    FractionNumber:
                                    (FractionNumber|IntegralNumber)
                                ):
                                NaN
                            )
                        )
                    )
                )
            ):
            (BOOST_METAMATH_IS_EXPONENT(BOOST_PP_CAT(a,N)))?
            Exponent:
            (
                (BOOST_METAMATH_IS_SIGN(BOOST_PP_CAT(a,N)) && BOOST_PP_CAT(status_,N_1)==Exponent)?
                ExponentSign:
                BOOST_METAMATH_IS_END(BOOST_PP_CAT(a,N))?CompleteNumber:NaN
            )                
        )
    ));

    BOOST_STATIC_CONSTANT(boost::uint32_t,BOOST_PP_CAT(result_b_,N)=(
        (((BOOST_PP_CAT(status_,N) & IntegralNumber) || (BOOST_PP_CAT(status_,N) &FractionNumber)) && !(BOOST_PP_CAT(status_,N) &Overflow))?
        (((BOOST_PP_CAT(result_b_,N_1) <<3)&0x7fffffff)+((BOOST_PP_CAT(result_b_,N_1) <<1)&0x7fffffff)+BOOST_METAMATH_GET_DIGIT(BOOST_PP_CAT(a,N))):
        (BOOST_PP_CAT(status_,N) &NaN)?0:BOOST_PP_CAT(result_b_,N_1)));

    BOOST_STATIC_CONSTANT(boost::uint32_t,BOOST_PP_CAT(result_a_,N)=(
        (((BOOST_PP_CAT(status_,N) &IntegralNumber) || (BOOST_PP_CAT(status_,N) &FractionNumber)) && !(BOOST_PP_CAT(status_,N) &Overflow))?
        ((
            (BOOST_PP_CAT(result_a_,N_1)<<3)+
            (BOOST_PP_CAT(result_a_,N_1)<<1)+
            ((BOOST_PP_CAT(result_b_,N_1)&0x7fffffff)>>28)+
            ((BOOST_PP_CAT(result_b_,N_1)&0x7fffffff)>>30)+
            (BOOST_PP_CAT(result_b_,N)>>31))):
        (BOOST_PP_CAT(status_,N) &NaN)?0:BOOST_PP_CAT(result_a_,N_1)));
        

    BOOST_STATIC_CONSTANT(boost::int16_t,BOOST_PP_CAT(exp10_overflow_,N)=(
        (BOOST_PP_CAT(status_,N)==(IntegralNumber|Overflow))?
        BOOST_PP_CAT(exp10_overflow_,N_1)+1:
        (   
            ((BOOST_PP_CAT(status_,N)&FractionNumber) && !(BOOST_PP_CAT(status_,N)&Overflow))?
            BOOST_PP_CAT(exp10_overflow_,N_1)-1:BOOST_PP_CAT(exp10_overflow_,N_1)
        )));
    BOOST_STATIC_CONSTANT(boost::int16_t,BOOST_PP_CAT(exp10_,N)=(
        (BOOST_PP_CAT(status_,N)==ExponentSign)?
        ((BOOST_PP_CAT(a,N)=='-')?-1:1):
        (
            BOOST_PP_CAT(status_,N)==ExponentNumber?
            (
                (BOOST_PP_CAT(status_,N_1)==ExponentSign)?
                BOOST_PP_CAT(exp10_,N_1)*BOOST_METAMATH_GET_DIGIT(BOOST_PP_CAT(a,N)):
                (
                    (BOOST_PP_CAT(exp10_,N_1)<0)?
                    -(((-BOOST_PP_CAT(exp10_,N_1))<<3)+((-BOOST_PP_CAT(exp10_,N_1))<<1)+BOOST_METAMATH_GET_DIGIT(BOOST_PP_CAT(a,N))):
                    ((BOOST_PP_CAT(exp10_,N_1)<<3)+(BOOST_PP_CAT(exp10_,N_1)<<1)+BOOST_METAMATH_GET_DIGIT(BOOST_PP_CAT(a,N)))
                )
            ):
            BOOST_PP_CAT(exp10_,N_1)
        )));

#undef N
#undef N_1

#endif
