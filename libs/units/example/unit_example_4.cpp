// mcs::units - A C++ library for zero-overhead dimensional analysis and 
// unit/quantity manipulation and conversion
//
// Copyright (C) 2003-2007 Matthias Christian Schabel
// Copyright (C) 2007 Steven Watanabe
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

/** 
\file
    
\brief unit_example_4.cpp

\detailed
More extensive quantity tests.

Output:
@verbatim

//[unit_example_4_output_1
S1 :    2
X1 :    2
X2 :    (4/3)
U1 :    m kg s^-2
U2 :    m^2 kg s^-2
Q1 :    1 m kg s^-2
Q2 :    2 m^2 kg s^-2
//]

//[unit_example_4_output_2
U1*S1 : 2 m kg s^-2
S1*U1 : 2 m kg s^-2
U1/S1 : 0.5 m kg s^-2
S1/U1 : 2 m^-1 kg^-1 s^2
//]

//[unit_example_4_output_3
U1+U1 : m kg s^-2
U1-U1 : m kg s^-2
U1*U1 : m^2 kg^2 s^-4
U1/U1 : dimensionless
U1*U2 : m^3 kg^2 s^-4
U1/U2 : m^(-1)
U1^X  : m^2 kg^2 s^-4
X1vU1 : m^(1/2) kg^(1/2) s^-1
U1^X2 : m^(4/3) kg^(4/3) s^(-8/3)
X2vU1 : m^(3/4) kg^(3/4) s^(-3/2)
//]

//[unit_example_4_output_4
Q1*S1 : 2 m kg s^-2
S1*Q1 : 2 m kg s^-2
Q1/S1 : 0.5 m kg s^-2
S1/Q1 : 2 m^-1 kg^-1 s^2
//]

//[unit_example_4_output_5
U1*Q1 : 1 m^2 kg^2 s^-4
Q1*U1 : 1 m^2 kg^2 s^-4
U1/Q1 : 1 dimensionless
Q1/U1 : 1 dimensionless
//]

//[unit_example_4_output_6
+Q1   : 1 m kg s^-2
-Q1   : -1 m kg s^-2
Q1+Q1 : 2 m kg s^-2
Q1-Q1 : 0 m kg s^-2
Q1*Q1 : 1 m^2 kg^2 s^-4
Q1/Q1 : 1 dimensionless
Q1*Q2 : 2 m^3 kg^2 s^-4
Q1/Q2 : 0.5 m^-1
Q1^X1 : 1 m^2 kg^2 s^-4
X1vQ1 : 1 m^(1/2) kg^(1/2) s^-1
Q1^X2 : 1 m^(4/3) kg^(4/3) s^(-8/3)
X2vQ1 : 1 m^(3/4) kg^(3/4) s^(-3/2)
//]

//[unit_example_4_output_7
l1 == l2    false
l1 != l2    true
l1 <= l2    true
l1 < l2     true
l1 >= l2    false
l1 > l2     false
//]

dimless = 1

//[unit_example_4_output_8
v1 = 2 m s^-1
//]

//[unit_example_4_output_9
F  = 1 m kg s^-2
dx = 1 m
E  = 1 m^2 kg s^-2
//]

//[unit_example_4_output_10
r = 5e-07 m
P = 101325 m^-1 kg s^-2
V = 5.23599e-19 m^3
T = 310 K
n = 2.05835e-17 mol
R = 8.31447 m^2 kg s^-2 K^-1 mol^-1
//]

//[unit_example_4_output_11
theta            = 0.375 rd
sin(theta)       = 0.366273 dimensionless
asin(sin(theta)) = 0.375 rd
//]

//[unit_example_4_output_12
V   = (12.5,0) m^2 kg s^-3 A^-1
I   = (3,4) A
Z   = (1.5,-2) m^2 kg s^-3 A^-2
I*Z = (12.5,0) m^2 kg s^-3 A^-1
//]

//[unit_example_4_output_13
x+y-w         = 0.48(+/-0.632772) m
w*x           = 9.04(+/-0.904885) m^2
x/y           = 0.666667(+/-0.149071) dimensionless
//]

//[unit_example_4_output_14
w*y^2/(u*x)^2 = 10.17(+/-3.52328) m^-1
w/(u*x)^(1/2) = 3.19612(+/-0.160431) dimensionless
//]

@endverbatim
**/

#include <cmath>
#include <complex>
#include <iostream>
#include <algorithm>
#include <sstream>

#include <boost/units/io.hpp>
#include <boost/units/systems/si.hpp>
#include <boost/units/systems/trig.hpp>
#include <boost/units/systems/si/codata/physico-chemical_constants.hpp>

#include "measurement.hpp"

namespace boost {

namespace units {

/*
//[unit_example_4_function_snippet_1
/// sin takes a quantity and returns a dimensionless quantity
template<class System,class Y>
quantity<unit<dimensionless_type,System>,Y>
sin(const quantity<unit<plane_angle_type,System>,Y>& theta)
{
    return quantity<unit<dimensionless_type,System>,Y>(std::sin(theta.value()));
}
//]

//[unit_example_4_function_snippet_2
/// asin takes a dimensionless quantity and returns a quantity
template<class System,class Y>
quantity<unit<plane_angle_type,System>,Y>
asin(const quantity<unit<dimensionless_type,System>,Y>& val)
{
    typedef quantity<unit<plane_angle_type,System>,Y>    quantity_type;
    
    return quantity_type::from_value(std::asin(val.value()));
}
//]
*/

//[unit_example_4_function_snippet_3
/// the physical definition of work - computed for an arbitrary unit system 
template<class System,class Y>
quantity<unit<energy_type,System>,Y> 
work(quantity<unit<force_type,System>,Y> F,
     quantity<unit<length_type,System>,Y> dx)
{
    return F*dx;
}
//]

//[unit_example_4_function_snippet_4
/// the ideal gas law in SI units
template<class Y>
quantity<SI::amount,Y> 
idealGasLaw(const quantity<SI::pressure,Y>& P,
            const quantity<SI::volume,Y>& V,
            const quantity<SI::temperature,Y>& T)
{
    using namespace boost::units::SI;
    
    #if BOOST_UNITS_HAS_TYPEOF 
    return (P*V/(constants::CODATA::R*T));
    #else
    return P*V/(8.314472*(joules/(kelvin*mole))*T);
    #endif // BOOST_UNITS_HAS_TYPEOF
}
//]

} // namespace units

} // namespace boost

int main()
{    
    using namespace boost::units;
    using namespace boost::units::SI;

    std::stringstream sstream1, sstream2;

    {
    //[unit_example_4_snippet_1
    /// scalar
    const double    s1 = 2;
    
    const long                  x1 = 2;
    const static_rational<4,3>  x2;
    
    /// define some units
    force       u1 = newton;
    energy      u2 = joule;
    
    /// define some quantities
    quantity<force>      q1(1.0*u1);
    quantity<energy>     q2(2.0*u2);
    //]
    
    /// check scalar, unit, and quantity io
    sstream1  << "S1 :    " << s1 << std::endl
              << "X1 :    " << x1 << std::endl
              << "X2 :    " << x2 << std::endl
              << "U1 :    " << u1 << std::endl
              << "U2 :    " << u2 << std::endl
              << "Q1 :    " << q1 << std::endl
              << "Q2 :    " << q2 << std::endl
              << std::endl;
    
    /// check scalar-unit algebra
    sstream1  //<< "U1+S1 : " << u1+s1 << std::endl    // illegal
              //<< "S1+U1 : " << s1+u1 << std::endl    // illegal
              //<< "U1-S1 : " << u1-s1 << std::endl    // illegal
              //<< "S1-U1 : " << s1-u1 << std::endl    // illegal
              << "U1*S1 : " << u1*s1 << std::endl
              << "S1*U1 : " << s1*u1 << std::endl
              << "U1/S1 : " << u1/s1 << std::endl
              << "S1/U1 : " << s1/u1 << std::endl
              << std::endl;
    
    /// check unit-unit algebra
    sstream1  << "U1+U1 : " << u1+u1 << std::endl
              << "U1-U1 : " << u1-u1 << std::endl
              << "U1*U1 : " << u1*u1 << std::endl
              << "U1/U1 : " << u1/u1 << std::endl
              //<< "U1+U2 : " << u1+u2 << std::endl     // illegal
              //<< "U1-U2 : " << u1-u2 << std::endl     // illegal
              << "U1*U2 : " << u1*u2 << std::endl
              << "U1/U2 : " << u1/u2 << std::endl
              << "U1^X  : " << pow<2>(u1) << std::endl
              << "X1vU1 : " << root<2>(u1) << std::endl
              << "U1^X2 : " << pow<static_rational<4,3> >(u1) << std::endl
              << "X2vU1 : " << root<static_rational<4,3> >(u1) << std::endl
              << std::endl;
    
    /// check scalar-quantity algebra
    sstream1  //<< "Q1+S1 : " << q1+s1 << std::endl    // illegal
              //<< "S1+Q1 : " << s1+q1 << std::endl    // illegal
              //<< "Q1-S1 : " << q1-s1 << std::endl    // illegal
              //<< "S1-Q1 : " << s1-q1 << std::endl    // illegal
              << "Q1*S1 : " << q1*s1 << std::endl
              << "S1*Q1 : " << s1*q1 << std::endl
              << "Q1/S1 : " << q1/s1 << std::endl
              << "S1/Q1 : " << s1/q1 << std::endl
              << std::endl;
    
    /// check unit-quantity algebra
    sstream1  //<< "U1+Q1 : " << u1+q1 << std::endl    // illegal
              //<< "Q1+U1 : " << q1+u1 << std::endl    // illegal
              //<< "U1-Q1 : " << u1-q1 << std::endl    // illegal
              //<< "Q1-U1 : " << q1-u1 << std::endl    // illegal
              << "U1*Q1 : " << u1*q1 << std::endl
              << "Q1*U1 : " << q1*u1 << std::endl
              << "U1/Q1 : " << u1/q1 << std::endl
              << "Q1/U1 : " << q1/u1 << std::endl
              << std::endl;
    
    /// check quantity-quantity algebra
    sstream1  << "+Q1   : " << +q1 << std::endl
              << "-Q1   : " << -q1 << std::endl
              << "Q1+Q1 : " << q1+q1 << std::endl
              << "Q1-Q1 : " << q1-q1 << std::endl
              << "Q1*Q1 : " << q1*q1 << std::endl
              << "Q1/Q1 : " << q1/q1 << std::endl
              //<< "Q1+Q2 : " << q1+q2 << std::endl    // illegal
              //<< "Q1-Q2 : " << q1-q2 << std::endl    // illegal
              << "Q1*Q2 : " << q1*q2 << std::endl
              << "Q1/Q2 : " << q1/q2 << std::endl
              << "Q1^X1 : " << pow<2>(q1) << std::endl
              << "X1vQ1 : " << root<2>(q1) << std::endl
              << "Q1^X2 : " << pow<static_rational<4,3> >(q1) << std::endl
              << "X2vQ1 : " << root<static_rational<4,3> >(q1) << std::endl
              << std::endl;
    
    //[unit_example_4_snippet_2
    /// check comparison tests
    quantity<length>    l1(1.0*meter),
                        l2(2.0*meters);
    //]
                        
    sstream1  << std::boolalpha
              << "l1 == l2" << "\t" << (l1 == l2) << std::endl
              << "l1 != l2" << "\t" << (l1 != l2) << std::endl
              << "l1 <= l2" << "\t" << (l1 <= l2) << std::endl
              << "l1 < l2 " << "\t" << (l1  < l2) << std::endl
              << "l1 >= l2" << "\t" << (l1 >= l2) << std::endl
              << "l1 > l2 " << "\t" << (l1  > l2) << std::endl
              << std::endl;
    
    //[unit_example_4_snippet_3
    /// check implicit unit conversion from dimensionless to value_type  
    const double    dimless = (q1/q1);
    //]
    
    sstream1 << "dimless = " << dimless << std::endl
             << std::endl;
             
    quantity<velocity>  v1 = 2.0*meters/second;
        
    sstream1 << "v1 = " << v1 << std::endl
              << std::endl;
    
    //[unit_example_4_snippet_4
    /// test calcuation of work
    quantity<force>       F(1.0*newton);
    quantity<length>      dx(1.0*meter);
    quantity<energy>      E(work(F,dx));
    //]
    
    sstream1  << "F  = " << F << std::endl
              << "dx = " << dx << std::endl
              << "E  = " << E << std::endl
              << std::endl;
    
    {
    //[unit_example_4_snippet_5
    /// test ideal gas law
    quantity<temperature>   T = (273.+37.)*kelvin;
    quantity<pressure>      P = 1.01325e5*pascals;
    quantity<length>        r = 0.5e-6*meters;
    quantity<volume>        V = (4.0/3.0)*3.141592*pow<3>(r);
    quantity<amount>        n(idealGasLaw(P,V,T));
    //]
    
    sstream1  << "r = " << r << std::endl
              << "P = " << P << std::endl
              << "V = " << V << std::endl
              << "T = " << T << std::endl
              << "n = " << n << std::endl
              #if BOOST_UNITS_HAS_TYPEOF
              << "R = " << constants::CODATA::R << std::endl
              #else
              << "no typeof" << std::endl
              #endif // BOOST_UNITS_HAS_TYPEOF
              << std::endl;
    }         
    
    //[unit_example_4_snippet_6
    /// test trig stuff
    quantity<plane_angle>           theta = 0.375*radians;
    quantity<dimensionless>         sin_theta = sin(theta);
    quantity<plane_angle>           thetap = asin(sin_theta);
    //]
    
    sstream1  << "theta            = " << theta << std::endl
              << "sin(theta)       = " << sin_theta << std::endl
              << "asin(sin(theta)) = " << thetap << std::endl
              << std::endl;
    
    /// test implicit conversion of dimensionless to value
    double  tmp = sin_theta;
    
    tmp = sin_theta;
    
    /// test implicit conversion from value to dimensionless
    quantity<dimensionless>     tmpp = tmp;
    
    tmpp = tmp;
    
    /// check complex quantities
    typedef std::complex<double>    complex_type;
    
    //[unit_example_4_snippet_7
    quantity<electric_potential,complex_type>   v = complex_type(12.5,0.0)*volts;
    quantity<current,complex_type>              i = complex_type(3.0,4.0)*amperes;
    quantity<resistance,complex_type>           z = complex_type(1.5,-2.0)*ohms;
    //]
    
    sstream1  << "V   = " << v << std::endl
              << "I   = " << i << std::endl
              << "Z   = " << z << std::endl
              << "I*Z = " << i*z << std::endl
              << std::endl;
    
    /// check quantities using user-defined type encapsulating error propagation

    //[unit_example_4_snippet_8
    quantity<length,measurement<double> >   u(measurement<double>(1.0,0.0)*meters),
                                            w(measurement<double>(4.52,0.02)*meters),
                                            x(measurement<double>(2.0,0.2)*meters),
                                            y(measurement<double>(3.0,0.6)*meters);
    //]
                                        
    sstream1  << "x+y-w         = " << x+y-w << std::endl
              << "w*x           = " << w*x << std::endl
              << "x/y           = " << x/y << std::endl
              << "w*y^2/(u*x)^2 = " << w*y*y/pow<2>(u*x) << std::endl
              << "w/(u*x)^(1/2) = " << w/pow< static_rational<1,2> >(u*x) << std::endl
              << std::endl;
    }

    sstream2 << "S1 :    2" << std::endl;
    sstream2 << "X1 :    2" << std::endl;
    sstream2 << "X2 :    (4/3)" << std::endl;
    sstream2 << "U1 :    m kg s^-2" << std::endl;
    sstream2 << "U2 :    m^2 kg s^-2" << std::endl;
    sstream2 << "Q1 :    1 m kg s^-2" << std::endl;
    sstream2 << "Q2 :    2 m^2 kg s^-2" << std::endl;
    sstream2 << std::endl;
    sstream2 << "U1*S1 : 2 m kg s^-2" << std::endl;
    sstream2 << "S1*U1 : 2 m kg s^-2" << std::endl;
    sstream2 << "U1/S1 : 0.5 m kg s^-2" << std::endl;
    sstream2 << "S1/U1 : 2 m^-1 kg^-1 s^2" << std::endl;
    sstream2 << std::endl;
    sstream2 << "U1+U1 : m kg s^-2" << std::endl;
    sstream2 << "U1-U1 : m kg s^-2" << std::endl;
    sstream2 << "U1*U1 : m^2 kg^2 s^-4" << std::endl;
    sstream2 << "U1/U1 : dimensionless" << std::endl;
    sstream2 << "U1*U2 : m^3 kg^2 s^-4" << std::endl;
    sstream2 << "U1/U2 : m^-1" << std::endl;
    sstream2 << "U1^X  : m^2 kg^2 s^-4" << std::endl;
    sstream2 << "X1vU1 : m^(1/2) kg^(1/2) s^-1" << std::endl;
    sstream2 << "U1^X2 : m^(4/3) kg^(4/3) s^(-8/3)" << std::endl;
    sstream2 << "X2vU1 : m^(3/4) kg^(3/4) s^(-3/2)" << std::endl;
    sstream2 << std::endl;
    sstream2 << "Q1*S1 : 2 m kg s^-2" << std::endl;
    sstream2 << "S1*Q1 : 2 m kg s^-2" << std::endl;
    sstream2 << "Q1/S1 : 0.5 m kg s^-2" << std::endl;
    sstream2 << "S1/Q1 : 2 m^-1 kg^-1 s^2" << std::endl;
    sstream2 << std::endl;
    sstream2 << "U1*Q1 : 1 m^2 kg^2 s^-4" << std::endl;
    sstream2 << "Q1*U1 : 1 m^2 kg^2 s^-4" << std::endl;
    sstream2 << "U1/Q1 : 1 dimensionless" << std::endl;
    sstream2 << "Q1/U1 : 1 dimensionless" << std::endl;
    sstream2 << std::endl;
    sstream2 << "+Q1   : 1 m kg s^-2" << std::endl;
    sstream2 << "-Q1   : -1 m kg s^-2" << std::endl;
    sstream2 << "Q1+Q1 : 2 m kg s^-2" << std::endl;
    sstream2 << "Q1-Q1 : 0 m kg s^-2" << std::endl;
    sstream2 << "Q1*Q1 : 1 m^2 kg^2 s^-4" << std::endl;
    sstream2 << "Q1/Q1 : 1 dimensionless" << std::endl;
    sstream2 << "Q1*Q2 : 2 m^3 kg^2 s^-4" << std::endl;
    sstream2 << "Q1/Q2 : 0.5 m^-1" << std::endl;
    sstream2 << "Q1^X1 : 1 m^2 kg^2 s^-4" << std::endl;
    sstream2 << "X1vQ1 : 1 m^(1/2) kg^(1/2) s^-1" << std::endl;
    sstream2 << "Q1^X2 : 1 m^(4/3) kg^(4/3) s^(-8/3)" << std::endl;
    sstream2 << "X2vQ1 : 1 m^(3/4) kg^(3/4) s^(-3/2)" << std::endl;
    sstream2 << std::endl;
    sstream2 << "l1 == l2	false" << std::endl;
    sstream2 << "l1 != l2	true" << std::endl;
    sstream2 << "l1 <= l2	true" << std::endl;
    sstream2 << "l1 < l2 	true" << std::endl;
    sstream2 << "l1 >= l2	false" << std::endl;
    sstream2 << "l1 > l2 	false" << std::endl;
    sstream2 << std::endl;
    sstream2 << "dimless = 1" << std::endl;
    sstream2 << std::endl;
    sstream2 << "v1 = 2 m s^-1" << std::endl;
    sstream2 << std::endl;
    sstream2 << "F  = 1 m kg s^-2" << std::endl;
    sstream2 << "dx = 1 m" << std::endl;
    sstream2 << "E  = 1 m^2 kg s^-2" << std::endl;
    sstream2 << std::endl;
    sstream2 << "r = " << 5e-7 << " m" << std::endl;
    sstream2 << "P = 101325 m^-1 kg s^-2" << std::endl;
    sstream2 << "V = " << 5.23599e-19 << " m^3" << std::endl;
    sstream2 << "T = 310 K" << std::endl;
    sstream2 << "n = " << 2.05835e-17 << " mol" << std::endl;
    #if BOOST_UNITS_HAS_TYPEOF
    sstream2 << "R = 8.314472 m^2 kg s^-2 K^-1 mol^-1 (rel. unc. = " << 1.8e-6 << ")" << std::endl;
    #else
    sstream2 << "no typeof" << std::endl;
    #endif // BOOST_UNITS_HAS_TYPEOF
    sstream2 << std::endl;
    sstream2 << "theta            = " << 0.375 << " rad" << std::endl;
    sstream2 << "sin(theta)       = " << 0.366273 << " dimensionless" << std::endl;
    sstream2 << "asin(sin(theta)) = " << 0.375 << " rad" << std::endl;
    sstream2 << std::endl;
    sstream2 << "V   = (12.5,0) m^2 kg s^-3 A^-1" << std::endl;
    sstream2 << "I   = (3,4) A" << std::endl;
    sstream2 << "Z   = (1.5,-2) m^2 kg s^-3 A^-2" << std::endl;
    sstream2 << "I*Z = (12.5,0) m^2 kg s^-3 A^-1" << std::endl;
    sstream2 << std::endl;
    sstream2 << std::scientific << std::setprecision(1);

    sstream2 << "x+y-w         = 0.48(+/-0.632772) m" << std::endl;
    sstream2 << "w*x           = 9.04(+/-0.904885) m^2" << std::endl;
    sstream2 << "x/y           = 0.666667(+/-0.149071) dimensionless" << std::endl;
    sstream2 << "w*y^2/(u*x)^2 = 10.17(+/-3.52328) m^-1" << std::endl;
    sstream2 << "w/(u*x)^(1/2) = 3.19612(+/-0.160431) dimensionless" << std::endl;

    sstream2 << std::endl;

    std::string str1(sstream1.str());
    std::string str2(sstream2.str());

    std::cout << str1;
    
    if(str1 == str2) 
    {
        return(0);
    } 
    else 
    {
        std::cout << std::endl << str2 << std::endl;
        
        if(str1.size() < str2.size()) 
        {
            std::string::iterator iter = std::mismatch(str1.begin(), str1.end(), str2.begin()).first;
            
            std::cout << iter - str1.begin() << std::endl;
            std::cout << std::count(str1.begin(), iter, '\n') << std::endl;
        } 
        else 
        {
            std::string::iterator iter = std::mismatch(str2.begin(), str2.end(), str1.begin()).first;
            
            std::cout << iter - str2.begin() << std::endl;
            std::cout << std::count(str2.begin(), iter, '\n') << std::endl;
        }
        
        return(-1);
    }
}
