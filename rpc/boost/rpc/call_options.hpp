#ifndef BOOST_RPC_CALL_HEADER_HPP
#define BOOST_RPC_CALL_HEADER_HPP

#include <boost/cstdint.hpp>

namespace boost {
namespace rpc {

/// Holds the result marshaling and exception reporting options.
/** \todo Add unspecified options.
*/
struct call_options
{
    enum marshal_options
    {
        marshal_unspecified,
        no_results,
        completed_only,
        return_only,
        all_out_parameters,
    };
    enum exception_options
    {
        exception_unspecified,
        no_exception_handling
    };

    boost::int32_t marshal_option;
    boost::int32_t exception_option;

    call_options(marshal_options marshal_option_val = marshal_unspecified,
        exception_options exception_option_val = exception_unspecified)
        : marshal_option((boost::int32_t)marshal_option_val),
        exception_option((boost::int32_t)exception_option_val) {}
private:
    friend class boost::serialization::access;

    template<class Archive>
    void serialize(Archive &archive, const unsigned int version)
    {
        archive & marshal_option;
        archive & sync_option;
        archive & exception_option;
    }
};

} // namespace rpc
} // namespace boost

#endif // BOOST_RPC_CALL_HEADER_HPP