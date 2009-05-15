// Numeric Adaptor Library
//
// Copyright Barend Gehrels 2009, Geodan, Amsterdam
// Copyright Bruno Lalande 2009
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)


#ifndef NUMERIC_ADAPTOR_CLN_POLICY_HPP_
#define NUMERIC_ADAPTOR_CLN_POLICY_HPP_

#include <string>

#include <boost/numeric_adaptor/default_policy.hpp>

#include <cln/cln.h>
#include <cln/float.h>




/*
 CLN documentation, 3.2:
    As a user of CLN, you can forget about the differences between the four
    Floating-point types and just declare all your Foating-point variables
    as being of type cl_F.

*/
struct cln_policy : public default_policy<cln::cl_F>
{
    typedef cln::cl_F type;

    template <typename OtherType>
    static inline void set(type& value, OtherType const& v)
    {
        // Conversions from the C built-in type `double' are provided for the
        // classes cl_DF, cl_F, cl_R, cl_N and cl_number

        value = cln::cl_float(v, cln::float_format(256));
    }

    static inline void set(type& value, std::string const& v)
    {
        // CLN documentation 4.1.3:
        value = v.c_str();
    }


    static inline void sqrt(type& r, type const& a)
    {
        r = cln::sqrt(a);
    }

    static inline void cos(type& r, type const& a)
    {
        r = cln::cos(a);
    }

    static inline void sin(type& r, type const& a)
    {
        r = cln::sin(a);
    }


    template <typename OtherType>
    static inline OtherType big_numeric_cast(type const& b)
    {
        /*
            Conversions from the classes cl_I, cl_RA, cl_SF, cl_FF,
            cl_DF, cl_LF, cl_F and cl_R
            to the C built-in types `float' and `double' are provided
            through the functions

            float float_approx (type const& x)
            double double_approx (type const& x)

            Returns an approximation of x of C type ctype. If abs(x) is
            too close to 0 (underflow),
            0 is returned. If abs(x) is too large (overflow),
            an IEEE infinity is returned.
        */
        return OtherType(double_approx(b));
    }

};





#endif
