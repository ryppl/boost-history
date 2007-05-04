#ifndef BOOST_RPC_XML_ARCHIVE_HPP
#define BOOST_RPC_XML_ARCHIVE_HPP

#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>

namespace boost {
namespace rpc {

/// Allows the use of xml archives for marshaling.
struct xml_archive
{
    typedef boost::archive::xml_iarchive iarchive_type; ///< input archive type
    typedef boost::archive::xml_oarchive oarchive_type; ///< output archive type
};

} // namespace rpc
} // namespace boost

#endif // BOOST_RPC_XML_ARCHIVE_HPP
