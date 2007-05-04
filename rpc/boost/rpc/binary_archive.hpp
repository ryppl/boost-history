// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_RPC_BINARY_ARCHIVE_HPP
#define BOOST_RPC_BINARY_ARCHIVE_HPP

#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/rpc/serialize.hpp>

namespace boost {
namespace rpc {

/// Allows the use of binary archives for marshaling.
struct binary_archive
{
    typedef boost::archive::binary_iarchive iarchive_type; ///< input archive type
    typedef boost::archive::binary_oarchive oarchive_type; ///< output archive type
};

} // namespace rpc
} // namespace boost

#endif // BOOST_RPC_BINARY_ARCHIVE_HPP
