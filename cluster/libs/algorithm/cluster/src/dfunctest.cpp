#include "boost/algorithm/cluster/dist_func.hpp"
#include "boost/date_time/posix_time/posix_time.hpp"

#define ITER 500000
#define REITER 1000

template<typename T>
void runTests(const T& tup1, const T& tup2, int iter);

int main(int argc, char* argv[]) 
{
    using namespace boost;

    std::cout << "std::pair<double, double> test" << std::endl;
    runTests(std::make_pair(0,0), std::make_pair(0,1), ITER);
    std::cout << std::endl;

    std::cout << "tup2 test" << std::endl;
    runTests(make_tuple(0,0), make_tuple(0,1), ITER);
    std::cout << std::endl;
    
    std::cout << "tup3 test" << std::endl;
    runTests(make_tuple(0,0,0), make_tuple(0,1,2), ITER);
    std::cout << std::endl;
    
    std::cout << "tup4 test" << std::endl;
    runTests(make_tuple(0,0,0,0), make_tuple(0,1,2,3), ITER);
    std::cout << std::endl;

    std::cout << "tup5 test" << std::endl;
    runTests(make_tuple(0,0,0,0,0), make_tuple(0,1,2,3,4), ITER);
    std::cout << std::endl;

    std::cout << "tup6 test" << std::endl;
    runTests(make_tuple(0,0,0,0,0,0), make_tuple(0,1,2,3,4,5), ITER);
    std::cout << std::endl;

    std::cout << "tup7 test" << std::endl;
    runTests(make_tuple(0,0,0,0,0,0,0), make_tuple(0,1,2,3,4,5,6), ITER);
    std::cout << std::endl;
    
    std::cout << "tup8 test" << std::endl;
    runTests(make_tuple(0,0,0,0,0,0,0,0), make_tuple(0,1,2,3,4,5,6,7), ITER);
    std::cout << std::endl;

    std::cout << "tup9 test" << std::endl;
    runTests(make_tuple(0,0,0,0,0,0,0,0,0), make_tuple(0,1,2,3,4,5,6,7,8), ITER);
    std::cout << std::endl;

    std::cout << "tup10 test" << std::endl;
    runTests(make_tuple(0, 0, 0, 0, 0, 0, 0, 0, 0, 0), make_tuple(0, 1, 2, 3, 4, 5, 6, 7, 8, 9), ITER);
    std::cout << std::endl;
    
    return 0;
}

template<typename T>
void runTests(const T& tup1, const T& tup2, int iter) 
{

    boost::posix_time::ptime time1, time2;
    double d[REITER];

    // Euclid dist
    time1 = boost::posix_time::microsec_clock::local_time();
    for (int i = 0; i < 10*iter; ++i)
    {
        for(int j = 0; j < REITER; ++j)
            d[j] = boost::algorithm::cluster::euclid_dist(tup1, tup2);
    }
    time2 = boost::posix_time::microsec_clock::local_time();
    std::cout << "euclid_dist (" << d[0] << "): " << (time2 - time1) << std::endl;

    
    // Euclid dist (pretty impl)
    time1 = boost::posix_time::microsec_clock::local_time();
    for (int i = 0; i < iter; ++i)
    {
        for(int j = 0; j < REITER; ++j)
            d[j] = boost::algorithm::cluster::euclid_dist_pretty(tup1, tup2);
    }
    time2 = boost::posix_time::microsec_clock::local_time();
    std::cout << "euclid_dist_pretty (" << d[0] << "): " << (time2 - time1) << std::endl;
}