//////////////////////////////////////////////////////////////////////////////
// test::speed_common.cpp                                                   //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////
#include <algorithm>
#include <string>
#include <vector>
#include <boost/bind.hpp>
#include <libs/assign/test/speed_common.h>

// This file was provided by M.P.G 

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
rand_vec()
{
    std::vector<int> result(
        (std::size_t)rand(1, 100)
    );
    std::generate(
        result.begin(), 
        result.end(), 
        boost::bind(
            &rand, 
            0, 
            20
        )
    );
    return result;
}
