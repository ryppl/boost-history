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

#ifndef BOOST_MAPREDUCE_HASH_PARTITONER_HPP
#define BOOST_MAPREDUCE_HASH_PARTITONER_HPP
 
namespace boost {

namespace mapreduce {

struct hash_partitioner
{
    template<typename T>
    unsigned operator()(T const &key, unsigned partitions) const
    {
        boost::hash<T> hasher;
        return hasher(key) % partitions;
    }
};

}   // namespace mapreduce

}   // namespace boost

#endif  // BOOST_MAPREDUCE_HASH_PARTITONER_HPP
