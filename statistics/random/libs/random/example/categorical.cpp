#include <algorithm>
#include <boost/timer.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/chi_squared.hpp>
#include <boost/random/variate_generator.hpp>
#include <boost/random/categorical_distribution.hpp>
#include <boost/random/discrete_distribution_sw_2009.hpp>
#include <boost/binary_op/algorithm/sort_on_head.hpp>

void example_categorical(std::ostream& out){

    using namespace boost;
    typedef double              val_;
    typedef std::vector<val_>   vals_;

    typedef mt19937                                             urng_;
    typedef boost::random::chi_squared_distribution<val_>       rdist_;
    typedef boost::variate_generator<urng_&,rdist_>             vg_;
    typedef boost::random::categorical_distribution<>           cat_dist_;
    typedef range_size<vals_>::type                             size_;
    typedef boost::random::discrete_distribution<
        cat_dist_::result_type,
        val_
    > discr_dist_;
    
    const val_ df = 4.0;
    const size_ n = 1e4;
    const size_ m = 1e5;
    
    vals_ weights; weights.reserve(n);
    vals_ values(n);
    urng_ urng;
    rdist_ rdist( df );
    vg_ vg( urng, rdist);
    std::generate_n(
        std::back_inserter(weights),
        n,
        vg
    );
    vals_ sorted_weights = weights;

    timer t;
 
    out << ( format("--initialize, n = %1%")%n ).str() << std::endl;   
    out << "categorical : ";
    t.restart();
    binary_op::sort_on_head_greater(
        boost::begin(sorted_weights),
        boost::end(sorted_weights),
        boost::begin(values)
    );
    cat_dist_ cat_dist(sorted_weights);
    out << (format("t = %1%")%t.elapsed()).str();

    t.restart();
    out << std::endl << "discrete : ";
    discr_dist_ discr(
        boost::begin(weights),
        boost::end(weights)
    );
    out << (format("t = %1%")%t.elapsed()).str();

    out << std::endl << ( format("--sample, m = %1%")%m ).str() << std::endl;   
    out << "categorical : ";
    t.restart();
    for(unsigned i = 0; i<m; i++){
        cat_dist(urng);
    }
    out << (format("t = %1%")%t.elapsed()).str();
 
    out << std::endl << "discrete : ";
    t.restart();
    for(unsigned i = 0; i<m; i++){
        discr(urng);
    }
    out << (format("t = %1%")%t.elapsed()).str();

}


