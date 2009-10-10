//////////////////////////////////////////////////////////////////////////////
// statistics::detail::fusion::functor::iterator.hpp                        //
//                                                                          //
//  (C) Copyright 2009 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef  BOOST_STATISTICS_DETAIL_FUSION_ITERATOR_AT_KEY_HPP_ER_2009
#define  BOOST_STATISTICS_DETAIL_FUSION_ITERATOR_AT_KEY_HPP_ER_2009
#include <boost/utility/result_of.hpp>
#include <boost/iterator/transform_iterator.hpp>
#include <boost/iterator/iterator_traits.hpp>
#include <boost/statistics/detail/fusion/functor/at_key.hpp>

namespace boost{
namespace statistics{
namespace detail{
namespace fusion{
namespace iterator{

    template<typename It,typename K>
    struct meta_at_key
    {
        typedef statistics::detail::fusion::functor::at_key<K> f_;
        typedef typename iterator_reference<It>::type ref1_;
        
        // See boost.user mailing list
        // Subject: [transform_iterator]'s reference
        // 10/05/2009
        //
        // Note that at_key2 could also have been used
                
        typedef typename result_of<f_(ref1_)>::type ref2_;
        typedef boost::transform_iterator<f_,It,ref2_> type;
    
        static type make(It i){
            // don't use make_transform_iterator because not default.
            return type(i,f_());
        }
    
    };

    template<typename K,typename It>
    typename meta_at_key<It,K>::type
    make_at_key(It i){
        typedef meta_at_key<It,K> m_;
        return m_::make(i);
    }

}// functor
}// fusion
}// detail
}// statistics
}// boost

#endif
