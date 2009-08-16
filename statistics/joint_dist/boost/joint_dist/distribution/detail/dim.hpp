//////////////////////////////////////////////////////////////////////////////
// joint_dist::distribution::detail::dim.hpp                                //
//                                                                          //
//  (C) Copyright 2009 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_JOINT_DIST_DISTRIBUTION_DETAIL_DIM_HPP_ER_2009
#define BOOST_JOINT_DIST_DISTRIBUTION_DETAIL_DIM_HPP_ER_2009
#include <ostream>
#include <boost/format.hpp>

namespace boost{
namespace joint_dist{
namespace detail{

    struct dim{
        typedef std::size_t size_type;
        
        dim();
        dim(size_type n);
        
        const size_type& dimension()const;
        
        private:
        size_type n_;
    };

    std::ostream& operator<<(std::ostream& out,const dim& d);

    // Implementation //
    
    dim::dim():n_(0){};
    dim::dim(size_type n):n_(n){};
    const dim::size_type& dim::dimension()const{ return n_; }

    std::ostream& operator<<(std::ostream& out,const dim& d){
        return (out << (format("dim(%1%)")%d.dimension()).str());
    }


}// detail
}// joint_dist
}// boost

#endif