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


#include <boost/timer.hpp>


using namespace boost::numeric_adaptor;


template <typename T, typename Policy>
inline T heron(T const& ta, T const& tb, T const& tc)
{
    typedef numeric_adaptor<Policy> num;

    num a(ta);
    num b(tb);
    num c(tc);
    num s = (a + b + c) / num(2.0);
    return T(num::sqrt(s * (s - a) * (s - b) * (s - c)));
}


template <typename T>
inline T heron_direct(T const& a, T const& b, T const& c)
{
    T s = (a + b + c) / 2.0;
    return sqrt(s * (s - a) * (s - b) * (s - c));
}


int main()
{
    //long long int n = 1000000000;
    long long int n = 1000000;
    try
    {
        {
            boost::timer t;

            double s = 0;
            for (long long int i = 0; i < n; i++)
            {
                s += heron<double, ieee_policy<double> >(31622.77662, 0.000023, 31622.77661);
            }
            std::cout << "using policy, sum: " << s << " time: " << t.elapsed() << std::endl;
        }

        {
            boost::timer t;

            double s = 0;
            for (long long int i = 0; i < n; i++)
            {
                s += heron_direct<double>(31622.77662, 0.000023, 31622.77661);
            }
            std::cout << "using direct, sum: " << s << " time: " << t.elapsed() << std::endl;
        }




#if defined(HAVE_CLN)
        {
            boost::timer t;

            double s = 0;
            for (long long int i = 0; i < n; i++)
            {
                s += heron<double, cln_policy>(31622.77662, 0.000023, 31622.77661);
            }
            std::cout << "using CLN, sum: " << s << " time: " << t.elapsed() << std::endl;
        }
#endif

#if defined(HAVE_GMP)
        {
            boost::timer t;

            double s = 0;
            for (long long int i = 0; i < n; i++)
            {
                //s += heron<double, gmp_policy>(31622.77662, 0.000023, 31622.77661);
            }
            std::cout << "using GMP, sum: " << s << " time: " << t.elapsed() << std::endl;
        }
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
