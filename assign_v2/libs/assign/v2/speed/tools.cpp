///////////////////////////////////////////////////////////////////////////////
// Copyright 2010 Manuel Peinado Gallego                                     //
// Distributed under the Boost Software License, Version 1.0. (See           //
// accompanying file LICENSE_1_0.txt or copy at                              //
// http://www.boost.org/LICENSE_1_0.txt)                                     //
///////////////////////////////////////////////////////////////////////////////
#include <ctime>
#include <algorithm>
#include <string>
#include <vector>
#include <iterator>
#include <cstdlib>
#include <boost/bind.hpp>
#include <libs/assign/v2/speed/tools.h>

// http://code.google.com/p/truffle/source/browse/trunk/include/mpg/TimeIt.h
// http://code.google.com/p/truffle/source/browse/trunk/include/mpg/Random.h

// http://www.eternallyconfuzzled.com/arts/jsw_art_rand.aspx
inline double uniform_deviate ( int seed )
{
    return seed * ( 1.0 / ( RAND_MAX + 1.0 ) );
}
inline int rand(int M, int N) // Range [M..N)
{
    return int(M + uniform_deviate(std::rand()) * (N - M));
}
char rand_letter()
{
    return char(rand('a', 'z' + 1));
}

std::string rand_str(int len)
{
    std::string result;
    std::generate_n(std::back_inserter(result), len, &rand_letter);
    return result;
}

std::vector<int>
rand_vec(int max_n)
{
    std::vector<int> result(
        (std::size_t)mpg::rand(1, max_n)
    );
    std::generate(
        result.begin(),
        result.end(),
        boost::bind(
            &mpg::rand,
            0,
            20
        )
    );
    return result;
}

namespace mpg
{
    namespace detail
    {
        double clock_diff_to_sec(long clock_diff)
        {
            return double(clock_diff) / CLOCKS_PER_SEC;
        }

        template<class Proc>
        double time_it_impl(Proc proc, int N) // returns time in microseconds
        {
            std::clock_t const start = std::clock();
            for(int i = 0; i < N; ++i)
                proc();
            std::clock_t const end = std::clock();
            if(clock_diff_to_sec(end - start) < .2)
                return time_it_impl(proc, N * 5);
            return clock_diff_to_sec(end - start) * (1e6 / N);
        }

        template<class Proc, class Result>
        double time_it_impl(Proc proc, Result & result, int N) // returns time in microseconds
        {
            std::clock_t const start = std::clock();
            for(int i = 0; i < N; ++i)
                result = proc();
            std::clock_t const end = std::clock();
            if(clock_diff_to_sec(end - start) < .2)
                return time_it_impl(proc, result, N * 5);
            return clock_diff_to_sec(end - start) * (1e6 / N);
        }
    }

    template<class Proc>
    double time_it(Proc proc) // returns time in microseconds
    {
        return detail::time_it_impl(proc, 1);
    }

    template<class Proc, class Result>
    double time_it(Proc proc, Result & result) // returns time in microseconds
    {
        return detail::time_it_impl(proc, result, 1);
    }
}

namespace mpg
{
    inline double rand_dbl()
    {
        return double(::rand()) / RAND_MAX;
    }

    inline double rand_dbl(double M, double N)
    {
        return M + rand_dbl() * (N - M);
    }

    // http://www.eternallyconfuzzled.com/arts/jsw_art_rand.aspx
    inline int rand(int M, int N) // Range (M..N)
    {
        return int(M + std::rand() * ( 1.0 / ( RAND_MAX + 1.0 )) * (N - M));
    }

    inline char rand_letter()
    {
        return char(rand('a', 'z' + 1));
    }

    inline std::string rand_str(int len)
    {
        std::string result;
        result.reserve(len);
        for(int i = 0; i < len; ++i)
            result.push_back(rand_letter());
        return result;
    }
}


