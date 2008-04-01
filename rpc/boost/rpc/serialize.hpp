#ifndef BOOST_RPC_SERIALIZE
#define BOOST_RPC_SERIALIZE

#include <boost/fusion/functional/adapter/unfused_typed.hpp>
#include <boost/fusion/functional/adapter/fused.hpp>
#include <boost/fusion/sequence/generation/make_vector.hpp>
#include <boost/fusion/sequence/generation/vector_tie.hpp>
#include <boost/fusion/algorithm/iteration/for_each.hpp>
#include <boost/fusion/algorithm/transformation/zip.hpp>
#include <boost/function_types/parameter_types.hpp>
#include <boost/fusion/sequence/adapted/mpl.hpp>
#include <boost/mpl/push_front.hpp>

#include <boost/rpc/config.hpp>
#include <boost/detail/arity.hpp>
#include <boost/call_traits.hpp>
#include <boost/utility/enable_if.hpp>
#include <sstream>

namespace boost {
namespace rpc {
namespace detail {

template<typename ArchivePair>
struct archiver
{
    typename ArchivePair::oarchive_type &archive;

    archiver(typename ArchivePair::oarchive_type &archive) : archive(archive) {} 

    template <typename T>
    void operator()(T & val) const
    {
        archive & val;
    }
};

template<typename ArchivePair, int Args>
class fused_serialize
{
public:
    // Don't allow less than Args arguments
    template <class Seq>
    struct result
        : public boost::enable_if_c<Args==boost::fusion::result_of::size<Seq>::type::value, std::string>
    {};

    template <class Seq>
    std::string operator()(Seq const & s) const
    {
        std::stringstream stream(std::ios::in | std::ios::out | std::ios::binary);
        typename ArchivePair::oarchive_type archive(stream);
        using namespace boost::fusion;
        for_each(s, archiver<ArchivePair>(archive) );
        return stream.str();
    }
};

template<typename Id, typename ArchivePair, typename Signature>
struct serialize
    : public boost::fusion::unfused_typed<fused_serialize<ArchivePair,
    boost::function_types::function_arity<Signature>::value + 1>,
    typename boost::mpl::push_front<typename boost::function_types::parameter_types<Signature>, Id>::type>
{};


} // namespace detail
} // namespace rpc
} // namespace boost

#endif // BOOST_RPC_SERIALIZE
