///////////////////////////////////////////////////////////////////////////////
// view::algorithm::transform_column.hpp                              //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_VIEW_ALGORITHM_TRANSFORM_COLUMN_HPP_ER_2009
#define BOOST_VIEW_ALGORITHM_TRANSFORM_COLUMN_HPP_ER_2009
#include <iterator>
#include <boost/utility.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/concept_check.hpp>
#include <boost/iterator/iterator_concepts.hpp>

namespace boost{
namespace view{

    // Visits the subsequence defined by a stride and an offset, and transforms
    // it by f. 
    //
    // | b |___|___|___|___|___| ... |___| e | 
    // | 0 |___|___| 1 |___|___| ... |n-1|___| 
    //
    // A k-step iterator + algorithm would be more general, but until then...
    template<unsigned k,typename F,typename It,typename ItO>
    ItO transform_column(
        It b,   // first matrix elem
        It e,   // last matrix elem
        unsigned offset,
        F f, 
        ItO i_o // column 
    );

    template<unsigned k,typename F,typename It,typename ItO>
    ItO transform_to_column(
        It b,   // first of column
        It e,   // end of column
        unsigned offset,
        F f, 
        ItO i_o // first matrix elem
    );

    template<unsigned k,typename It,typename ItO>
    ItO copy_column(
        It b,
        It e,
        unsigned offset,
        ItO i_o
    ){
        return transform_column<k>(b, e, offset, boost::lambda::_1, i_o);
    }


    template<unsigned k,typename It,typename ItO>
    ItO copy_to_column(
        It b,
        It e,
        unsigned offset,
        ItO i_o
    ){
        return transform_to_column<k>(b, e, offset, boost::lambda::_1, i_o);
    }


    // Implementation //

    template<unsigned k,typename F,typename It,typename ItO>
    ItO transform_column(
        It b,
        It e,
        unsigned offset,
        F f, 
        ItO i_o
    ){
        BOOST_CONCEPT_ASSERT((boost_concepts::IncrementableIterator<It>));
        BOOST_CONCEPT_ASSERT((boost_concepts::IncrementableIterator<ItO>));
        // TODO allow Writable or back_inserter
        // BOOST_CONCEPT_ASSERT((boost_concepts::WritableIterator<ItO>));
    
        typedef typename iterator_difference<It>::type diff_;
        diff_ d = std::distance( b, e );
        BOOST_ASSERT(d>=0);
        if(d<offset){
            return i_o;
        }
        b = boost::next( b, offset );
        diff_ n = d / k;
        *i_o = f(*b);
        ++i_o;
        for(unsigned i = 1; i<n; ++i){
            b = boost::next( b, k );            
            *i_o = f(*b);
            ++i_o;
        }
        return i_o;
    }


    template<unsigned k,typename F,typename It,typename ItO>
    ItO transform_to_column(
        It b,
        It e,
        unsigned offset,
        F f, 
        ItO i_o
    ){
        BOOST_CONCEPT_ASSERT((boost_concepts::IncrementableIterator<It>));
        BOOST_CONCEPT_ASSERT((boost_concepts::IncrementableIterator<ItO>));
        // TODO allow std::back_inserter
        // BOOST_CONCEPT_ASSERT((boost_concepts::WritableIterator<It>));
    
        typedef typename iterator_difference<It>::type diff_;
        i_o = boost::next( i_o, offset );
        diff_ n = std::distance(b, e);
        *i_o = f( *b );
        ++b;
        for(unsigned i = 1; i<n; ++i){
            i_o = boost::next( i_o, k );            
            *i_o = f( *b );
            ++b;
        }
        return ++i_o;
    }

}// algorithm
}// boost

#endif
