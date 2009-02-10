#ifndef BOOST_ALGORITHM_L2_NORM_SQUARED_HPP_ER_2009
#define BOOST_ALGORITHM_L2_NORM_SQUARED_HPP_ER_2009
#include <numeric>
#include <boost/range.hpp>
#include <boost/range/value_type.hpp>

namespace boost{

class l2_norm_squared{
    public:
    l2_norm_squared(){}
    template<typename R0>
    typename range_value<R0>::type
    operator()(const R0& r0)const{
        typedef typename range_value<R0>::type result_type;
        return std::accumulate(
            boost::begin(r0),
            boost::end(r0),
            (result_type)(0),
            bin_op<result_type>()
        );
    };

    private:

    template<typename T>
    struct bin_op{
        bin_op(){}
        T operator()(T x,T y)const{ return x + y*y; }
    };
};

}
#endif
