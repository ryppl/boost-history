///////////////////////////////////////////////////////////////////////////////
// distribution::joint::example::random.cpp                                  //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#include <algorithm>
#include <boost/range.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/foreach.hpp>
#include <boost/statistics/detail/distribution_toolkit/distributions/normal/include.hpp>
#include <boost/statistics/detail/distribution/joint/distributions/iid/include.hpp>
//#include <boost/statistics/detail/distribution/joint/distributions/independent.hpp>

#include <boost/statistics/detail/distribution/joint/random/inner_sampler.hpp>

#include <libs/statistics/detail/distribution/joint/example/random.h>

void example_random(std::ostream& out){
    out << " -> example_random" << std::endl;
    out.flush();

    using namespace boost;
    namespace stat  = boost::statistics::detail;
    namespace dist  = stat::distribution;
    namespace tk    = dist::toolkit;
    namespace dj    = dist::joint;

    const long dim = 2;   // each sample of size dim
    const long n = 1e1;     // number of samples

    typedef double val_;
    typedef boost::mt19937                  urng_;
    typedef std::vector<val_>               row_;
    typedef std::vector<row_>               mat_;
    typedef math::normal_distribution<val_> dist_;
    typedef dj::iid<dist_>                  iid_;
    typedef dj::random_iid<dist_>           random_iid_;

    urng_ urng;

    mat_ mat(n,row_(dim)); 
    row_ lpdfs; lpdfs.reserve(n);

    iid_ iid( dist_(), dim );
    
    dist_ d;
    

    std::for_each(
        boost::begin(mat),
        boost::end(mat),
        dj::make_inner_sampler(
            urng,
            make_random(iid)
        )
    );
    
    out << (format("sample of %1% draws from ")%n).str() 
        << iid.description() 
        << std::endl;
        
    BOOST_FOREACH(const row_& r,mat){
        out << std::endl;
        BOOST_FOREACH(const val_& x,r){
            out << x << " ";
        }
    }
    
    out << "<-" << std::endl;

}
