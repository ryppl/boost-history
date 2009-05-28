// Numeric Adaptor Library testing suite
//
// Copyright Barend Gehrels 2009, Geodan Holding B.V. Amsterdam, the Netherlands.
// Copyright Bruno Lalande 2009
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)


#include <boost/numeric_adaptor/numeric_adaptor.hpp>
#include <boost/numeric_adaptor/ieee_policy.hpp>

#if defined(HAVE_GMP)
#  include <boost/numeric_adaptor/gmp_policy.hpp>
#endif

#if defined(HAVE_CLN)
#  include <boost/numeric_adaptor/cln_policy.hpp>
#endif


using namespace boost::numeric_adaptor;


template <typename T, typename Policy>
void heron(std::string const& header, T const& ta, T const& tb, T const& tc)
{
    typedef numeric_adaptor<Policy> num;

    num a = ta;
    num b = tb;
    num c = tc;
    num s = (a + b + c) / num(2.0);
    num C = num::sqrt(s * (s - a) * (s - b) * (s - c));
    std::cout << header << ": " << "C = " << T(C) << std::endl;
}


int main()
{
    try
    {
        heron<std::string, ieee_policy<float> > ("float  / string", "31622.77662", "0.000023", "31622.77661");
        heron<std::string, ieee_policy<double> >("double / string", "31622.77662", "0.000023", "31622.77661");

#if defined(HAVE_CLN)
        heron<std::string, cln_policy>("CLN    / string", "31622.77662", "0.000023", "31622.77661");
#endif

#if defined(HAVE_GMP)
        heron<std::string, gmp_policy>("GMP    / string", "31622.77662", "0.000023", "31622.77661");
#endif


    }
    catch(std::exception const& e)
    {
        std::cout << "Exception: " << e.what() << std::endl;
    }
    catch(...)
    {
        std::cout << "Exception, unknown" << std::endl;
    }

    return 0;
};
