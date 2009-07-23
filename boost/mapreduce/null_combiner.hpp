// Boost.MapReduce library
//
//  Copyright (C) 2009 Craig Henderson.
//  cdm.henderson@googlemail.com
//
//  Use, modification and distribution is subject to the
//  Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// For more information, see http://www.boost.org/libs/mapreduce/
//

#ifndef BOOST_MAPREDUCE_NULL_COMBINER_HPP
#define BOOST_MAPREDUCE_NULL_COMBINER_HPP
 
namespace boost {

namespace mapreduce {

struct null_combiner
{
    template<typename IntermediateValueType>
    void start(IntermediateValueType const &)
    { }

    template<typename IntermediateValueType, typename IntermediateStore>
    void finish(IntermediateValueType const &, IntermediateStore &)
    { }

    template<typename IntermediateValueType>
    void operator()(IntermediateValueType const &)
    { }
};

}   // namespace mapreduce

}   // namespace boost

#endif  // BOOST_MAPREDUCE_NULL_COMBINER_HPP
