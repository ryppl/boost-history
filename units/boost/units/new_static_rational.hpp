// mcs::units - A C++ library for zero-overhead dimensional analysis and 
// unit/quantity manipulation and conversion
//
// Copyright (C) 2003-2007 Matthias Christian Schabel
// Copyright (C) 2007 Steven Watanabe
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_STATIC_RATIONAL_HPP 
#define BOOST_STATIC_RATIONAL_HPP

#include <cmath>
#include <complex>

#include <boost/math/common_factor_ct.hpp>
#include <boost/mpl/arithmetic.hpp>
#include <boost/type_traits/is_same.hpp>

/// \file 
/// \brief Compile-time rational numbers and operators.

namespace boost {

namespace detail {

struct static_rational_tag {};

} // namespace detail

/// Compile time rational number.
/** 
This is an implementation of a compile time rational number, where @c static_rational<N,D> represents
a rational number with numerator @c N and denominator @c D. Because of the potential for ambiguity arising 
from multiple equivalent values of @c static_rational (e.g. @c static_rational<6,2>==static_rational<3>), 
static rationals should always be accessed through @c static_rational<N,D>::type. Template specialization 
prevents instantiation of zero denominators (i.e. @c static_rational<N,0>). The following compile-time 
arithmetic operators are provided for static_rational variables only (no operators are defined between 
long and static_rational):
    - @c static_negate
    - @c static_add
    - @c static_subtract
    - @c static_multiply
    - @c static_divide

Neither @c static_power nor @c static_root are defined for @c static_rational. This is because template types 
may not be floating point values, while powers and roots of rational numbers can produce floating point 
values. 
*/
template<I N,I D = 1,typename I = long>
class static_rational
{
    private:
        template<int_type Value>
        struct static_abs
        {
            BOOST_STATIC_CONSTANT(int_type,value) = Value < 0 ? -Value : Value;
        };
        
        /// greatest common divisor of N and D
        // need cast to signed because static_gcd returns unsigned long
        static const int_type   nabs = static_abs<N>::value,
                                dabs = static_abs<D>::value,
                                den = static_cast<int_type>(boost::math::static_gcd<nabs,dabs>::value),
                                Numerator = N/den,
                                Denominator = D/den;
        
    public: 
        // for mpl arithmetic support
        typedef detail::static_rational_tag tag;
        
        typedef static_rational<N,D,I>                      this_type;
        typedef I                                           int_type;
        typedef static_rational<Numerator,Denominator,I>    type;
                                 
        static int_type numerator()      { return Numerator; }
        static int_type denominator()    { return Denominator; }
        
        static_rational() { }
        //~static_rational() { }
        
//        // static rationals are implicitly convertible if reduced types are the same
//        template<integer_type NN,integer_type DD>
//        operator static_rational<NN,DD>()
//        {
//            BOOST_STATIC_ASSERT((boost::is_same<type,typename static_rational<NN,DD>::type>::value == true));
//            
//            return static_rational<NN,DD>();
//        }
};

// prohibit zero denominator
template<integer_type N,typename I> class static_rational<N,0,I>;

/// get decimal value of @c static_rational
template<class T,integer_type N,integer_type D>
typename divide_typeof_helper<T,T>::type 
value(const static_rational<N,D>& r)
{
    return T(N)/T(D);
}

/// raise @c int to a @c static_rational power
template<long N,long D> 
struct power_dimof_helper<int,static_rational<N,D> >                
{ 
    typedef double    type; 
    
    static type value(const int& x)  
    { 
        const static_rational<N,D>  rat;
        
        return std::pow(double(x),double(rat.numerator())/double(rat.denominator())); 
    }
};

/// raise @c float to a @c static_rational power
template<long N,long D> 
struct power_dimof_helper<float,static_rational<N,D> >                
{ 
    typedef double    type; 
    
    static type value(const float& x)  
    { 
        const static_rational<N,D>  rat;
        
        return std::pow(x,float(rat.numerator())/float(rat.denominator())); 
    }
};

/// raise @c double to a @c static_rational power
template<long N,long D> 
struct power_dimof_helper<double,static_rational<N,D> >                
{ 
    typedef double    type; 
    
    static type value(const double& x)  
    { 
        const static_rational<N,D>  rat;
        
        return std::pow(x,double(rat.numerator())/double(rat.denominator())); 
    }
};

/// raise @c std::complex<float> to a @c static_rational power
template<long N,long D> 
struct power_dimof_helper<std::complex<float>,static_rational<N,D> >  
{ 
    typedef std::complex<float>    type; 
    
    static type value(const std::complex<float>& x)  
    { 
        const static_rational<N,D>  rat;
        
        return std::pow(x,std::complex<float>(rat.numerator())/std::complex<float>(rat.denominator())); 
    }
};

/// raise @c std::complex<double> to a @c static_rational power
template<long N,long D> 
struct power_dimof_helper<std::complex<double>,static_rational<N,D> >  
{ 
    typedef std::complex<double>    type; 
    
    static type value(const std::complex<double>& x)  
    { 
        const static_rational<N,D>  rat;
        
        return std::pow(x,std::complex<double>(rat.numerator())/std::complex<double>(rat.denominator())); 
    }
};

/// take @c static_rational root of an @c int
template<long N,long D> 
struct root_typeof_helper<int,static_rational<N,D> >                
{ 
    typedef double    type; 
    
    static type value(const int& x)  
    { 
        const static_rational<N,D>  rat;
        
        return std::pow(double(x),double(rat.denominator())/double(rat.numerator())); 
    }
};

/// take @c static_rational root of a @c float
template<long N,long D> 
struct root_typeof_helper<float,static_rational<N,D> >                
{ 
    typedef float    type; 
    
    static type value(const float& x)  
    { 
        const static_rational<N,D>  rat;
        
        return std::pow(x,float(rat.denominator())/float(rat.numerator())); 
    }
};

/// take @c static_rational root of a @c double
template<long N,long D> 
struct root_typeof_helper<double,static_rational<N,D> >                
{ 
    typedef double    type; 
    
    static type value(const double& x)  
    { 
        const static_rational<N,D>  rat;
        
        return std::pow(x,double(rat.denominator())/double(rat.numerator())); 
    }
};

/// take @c static_rational root of a @c std::complex<float>
template<long N,long D> 
struct root_typeof_helper<std::complex<float>,static_rational<N,D> >  
{ 
    typedef std::complex<float>    type; 
    
    static type value(const std::complex<float>& x)  
    { 
        const static_rational<N,D>  rat;
        
        return std::pow(x,std::complex<float>(rat.denominator())/std::complex<float>(rat.numerator())); 
    }
};

/// take @c static_rational root of a @c std::complex<double>
template<long N,long D> 
struct root_typeof_helper<std::complex<double>,static_rational<N,D> >  
{ 
    typedef std::complex<double>    type; 
    
    static type value(const std::complex<double>& x)  
    { 
        const static_rational<N,D>  rat;
        
        return std::pow(x,std::complex<double>(rat.denominator())/std::complex<double>(rat.numerator())); 
    }
};

// need powers and roots of char, short, long long, unsigned integers...

/// raise a value to a @c static_rational power
template<class Rat,class Y>
typename power_dimof_helper<Y,Rat>::type
pow(const Y& x)
{
    return power_dimof_helper<Y,Rat>::value(x);
}

/// raise a value to an integer power
template<long N,class Y>
typename power_dimof_helper<Y,static_rational<N> >::type
pow(const Y& x)
{
    return power_dimof_helper<Y,static_rational<N> >::value(x);
}

/// take the @c static_rational root of a value
template<class Rat,class Y>
typename root_typeof_helper<Y,Rat>::type
root(const Y& x)
{
    return root_typeof_helper<Y,Rat>::value(x);
}

/// take the integer root of a value
template<long N,class Y>
typename root_typeof_helper<Y,static_rational<N> >::type
root(const Y& x)
{
    return root_typeof_helper<Y,static_rational<N> >::value(x);
}

namespace mpl {

template<>
struct plus_impl<boost::units::detail::static_rational_tag, boost::units::detail::static_rational_tag>
{
    template<class T0, class T1>
    struct apply {
        typedef typename boost::units::static_rational<
            T0::Numerator*T1::Denominator+T1::Numerator*T0::Denominator,
            T0::Denominator*T1::Denominator
        >::type type;
    };
};

template<>
struct minus_impl<boost::units::detail::static_rational_tag, boost::units::detail::static_rational_tag>
{
    template<class T0, class T1>
    struct apply {
        typedef typename boost::units::static_rational<
            T0::Numerator*T1::Denominator-T1::Numerator*T0::Denominator,
            T0::Denominator*T1::Denominator
        >::type type;
    };
};

template<>
struct times_impl<boost::units::detail::static_rational_tag, boost::units::detail::static_rational_tag>
{
    template<class T0, class T1>
    struct apply {
        typedef typename boost::units::static_rational<
            T0::Numerator*T1::Numerator,
            T0::Denominator*T1::Denominator
        >::type type;
    };
};

template<>
struct divides_impl<boost::units::detail::static_rational_tag, boost::units::detail::static_rational_tag>
{
    template<class T0, class T1>
    struct apply {
        typedef typename boost::units::static_rational<
            T0::Numerator*T1::Denominator,
            T0::Denominator*T1::Numerator
        >::type type;
    };
};

template<>
struct negate_impl<boost::units::detail::static_rational_tag>
{
    template<class T0>
    struct apply {
        typedef typename boost::units::static_rational<-T0::Numerator,T0::Denominator>::type type;
    };
};

} // namespace mpl

} // namespace boost

#endif // BOOST_STATIC_RATIONAL_HPP
