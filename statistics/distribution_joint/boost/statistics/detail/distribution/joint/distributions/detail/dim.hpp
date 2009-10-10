//////////////////////////////////////////////////////////////////////////////
// distribution::joint::distributions::detail::dim.hpp                       //
//                                                                          //
//  (C) Copyright 2009 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_STATISTICS_DETAIL_DISTRIBUTION_JOINT_DISTRIBUTION_DETAIL_DIM_HPP_ER_2009
#define BOOST_STATISTICS_DETAIL_DISTRIBUTION_JOINT_DISTRIBUTION_DETAIL_DIM_HPP_ER_2009
#include <ostream>
#include <boost/format.hpp>

namespace boost{
namespace statistics{
namespace detail{
namespace distribution{
namespace joint{
namespace detail{

    class dim{
        public:
        typedef std::size_t size_type;
        
        dim():n_(0){};
        dim(size_type n):n_(n){};
        const size_type& dimension()const{ return n_; }
        
        friend std::ostream& operator<<(
            std::ostream& out,
            const dim& d
        )
        {
            return (out << (boost::format("dim(%1%)")%d.dimension()).str());
        }
                
        private:
        size_type n_;
    };

}// detail
}// joint
}// distribution
}// detail
}// statistics
}// boost

#endif