//////////////////////////////////////////////////////////////////////////////
// cross_validation::extractor::meta_iterator.hpp                           //
//                                                                          //
//  (C) Copyright 2009 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef  BOOST_STATISTICS_DETAIL_CROSS_VALIDATION_EXTRACTOR_META_ITERATOR_HPP_ER_2009
#define  BOOST_STATISTICS_DETAIL_CROSS_VALIDATION_EXTRACTOR_META_ITERATOR_HPP_ER_2009
#include <boost/concept/assert.hpp>
#include <boost/iterator/iterator_traits.hpp>
#include <boost/iterator/transform_iterator.hpp>
#include <boost/statistics/detail/cross_validation/extractor/concept.hpp>
#include <boost/statistics/detail/cross_validation/extractor/identity.hpp>

namespace boost{
namespace statistics{
namespace detail{
namespace cross_validation{
namespace extractor{
namespace meta{

    template<typename F,typename It>
    struct iterator{
        
        typedef transform_iterator<F,It> type;
        
        typedef typename iterator_value<It>::type unit_;
        
        static type make(const F& f, It i){
        
            BOOST_CONCEPT_ASSERT((
                Concept<F,unit_>
            ));
        
            return make_tranform_iterator(i,f);
        };
    };
    
    template<typename It>
    struct iterator<identity,It>{
        typedef It type;
        
        static type make(const identity&,It i){
            return i;
        };
    
    };
    
}// meta    
}// iterator
}// cross_validation
}// detail
}// statistics
}// boost

#endif