///////////////////////////////////////////////////////////////////////////////
// joint_dist::example::example_1.h                                          //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#include <boost/range.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/foreach.hpp>
#include <boost/vector_space/functional/equal.hpp>
#include <boost/standard_distribution/distributions/normal.hpp>
// This must precede joint_dist
#include <boost/dist_random/distributions/normal.hpp>
#include <boost/joint_dist/distributions/iid.hpp>
#include <boost/joint_dist/distributions/independent.hpp>
#include <boost/joint_dist/random/generate_n.hpp>

void example_example_1(std::ostream& out){
    out << " -> example_example_1";
    using namespace boost;

    const long dim = 2;   // each sample of size dim
    const long n = 1e1;     // number of samples

    typedef double val_;
    typedef boost::mt19937                  urng_;
    typedef std::vector<val_>               row_;
    typedef std::vector<row_>               mat_;
    typedef range_iterator<mat_>            it_row_;
    typedef math::normal_distribution<val_> dist_;
    typedef std::vector<dist_>              dists_;
    typedef joint_dist::iid<dist_>          iid_;
    typedef joint_dist::independent<dists_> ind_;

    urng_ urng;
    urng_ urng2 = urng;

    mat_ mat(n,row_(dim)); 
    row_ lpdfs; lpdfs.reserve(n);

    iid_ iid( dist_(), dim );
    ind_ ind( dists_(dim,iid.element()) ); //so they are the same

    generate_n(
        boost::begin(mat),
        std::back_inserter(lpdfs),
        n,
        iid,
        urng
    );

    BOOST_FOREACH(const row_& r,mat){
        out << std::endl;
        BOOST_FOREACH(const val_& x,r){
            out << x << " ";
        }
    }

    row_ lpdfs2; lpdfs2.reserve(n);
    generate_n(
        boost::begin(mat),
        std::back_inserter(lpdfs2),
        n,
        ind,
        urng2
    );

    out << std::endl;
    BOOST_FOREACH(const row_& r,mat){
        out << std::endl;
        BOOST_FOREACH(const val_& x,r){
            out << x << " ";
        }
    }

    typedef vector_space::equal<row_&> vs_eq_;
    
    vs_eq_ vs_eq(lpdfs);
    
    //BOOST_ASSERT(vs_eq(lpdfs2)); // TODO don't agree. Bug?

    out << "<-";
}