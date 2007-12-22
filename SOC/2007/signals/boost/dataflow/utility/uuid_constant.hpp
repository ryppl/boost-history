// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_DATAFLOW_UTILITY_UUID_CONSTANT_HPP
#define BOOST_DATAFLOW_UTILITY_UUID_CONSTANT_HPP

#include <boost/mpl/and.hpp>
#include <boost/mpl/equal_to.hpp>
#include <boost/mpl/integral_c.hpp>
#include <stdint>


namespace boost { namespace dataflow { namespace utility {

template<uint32_t Word1, uint32_t Word2, uint32_t Word3, uint32_t Word4>
struct uuid_constant
{
    typedef mpl::integral_c<uint32_t, Word1> word1;
    typedef mpl::integral_c<uint32_t, Word2> word2;
    typedef mpl::integral_c<uint32_t, Word3> word3;
    typedef mpl::integral_c<uint32_t, Word4> word4;
};

template<typename Uuid1, typename Uuid2>
struct is_same_uuid
    : public typename mpl::and_<
        mpl::equal_to<Uuid1::word1, Uuid2::word1>,
        mpl::equal_to<Uuid1::word2, Uuid2::word2>,
        mpl::equal_to<Uuid1::word3, Uuid2::word3>,
        mpl::equal_to<Uuid1::word4, Uuid2::word4> >::type
{};

}}} // namespace boost::dataflow::utility

#endif // BOOST_DATAFLOW_UTILITY_UUID_CONSTANT_HPP