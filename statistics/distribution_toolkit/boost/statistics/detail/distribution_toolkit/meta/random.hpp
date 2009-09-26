///////////////////////////////////////////////////////////////////////////////
// distribution_toolkit::meta::random.hpp                                    //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_STATISTICS_DETAIL_DISTRIBUTION_TOOLKIT_META_RANDOM_HPP_ER_2009
#define BOOST_STATISTICS_DETAIL_DISTRIBUTION_TOOLKIT_META_RANDOM_HPP_ER_2009
#include <boost/utility/enable_if.hpp>
#include <boost/statistics/detail/distribution_toolkit/meta/is_scalar_distribution.hpp>

namespace boost{
namespace statistics{
namespace detail{
namespace distribution_toolkit{
namespace meta{

    template<typename D>
    struct random{};

    // Specialization for each D has to model RandomDistribution 
    // http://www.boost.org/doc/libs/1_40_0/libs/random/random-concepts.html
    // and provide static type make(const D& )

}// meta
}// distribution_toolkit
}// detail
}// statistics


    template<typename D>
    typename lazy_enable_if<
        statistics::detail::distribution_toolkit::meta::
            is_scalar_distribution<D>,
        statistics::detail::distribution_toolkit::meta::random<D> 
    >::type
    make_random(const D& d){
        typedef statistics::detail::distribution_toolkit::meta::random<D> meta_;
        return meta_::make(d);
    }

}// boost

#endif