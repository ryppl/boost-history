#ifndef BOOST_RPC_TEXT_ARCHIVE_HPP
#define BOOST_RPC_TEXT_ARCHIVE_HPP

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

namespace boost {
namespace rpc {

/// Allows the use of text archives for marshaling.
struct text_archive
{
    typedef boost::archive::text_iarchive iarchive_type; ///< input archive type
    typedef boost::archive::text_oarchive oarchive_type; ///< output archive type
};

} // namespace rpc
} // namespace boost

#endif // BOOST_RPC_TEXT_ARCHIVE_HPP
