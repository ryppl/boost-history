// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_RPC_REGISTRY_HPP
#define BOOST_RPC_REGISTRY_HPP

#include <boost/ptr_container/ptr_map.hpp>
#include <boost/call_traits.hpp>
#include <boost/rpc/function.hpp>
#include <boost/rpc/marshaled.hpp>

namespace boost {
namespace rpc {

/// Contains a collection of functions mapped by key.
/**   \param Id Type used as key for the functions.
   \param ArchivePair Pair of Boost.Serialization archives to be used for marshaling.
  \sa rpc::binary_archive, rpc::text_archive, rpc::xml_archive
*/
template<typename Id, typename ArchivePair = binary_archive>
class registry
{
public:
    typedef Id id_type;
    typedef ArchivePair archive_type;

    /// Adds a function and its id to the registry.
    /** \param id Id used to access the function
        \param f Function to be added.
        \todo figure out how to use call traits param type for
            id so ptr_map doesn't complains on Id=int
    */
    template<typename Signature>
    void set(Id id, const boost::function<Signature> &f)
    {
       reg.insert(id, new marshaled<Signature>(f));
    }

    /// Executes a function call.
    /** \param id_and_params Marshaled id of the function to call and the parameters.
        \param options Options specifying the results to be marshaled back.
        \param result A string to contain the marshaled results.
    */
    void operator()(const std::string &id_and_params, const call_options &options,
        std::string *result = NULL)
    {
        // prepare input archive to extract id and parameters
        std::stringstream p_stream(id_and_params, std::ios::in | std::ios::out | std::ios::binary);
        typename archive_type::iarchive_type p_archive(p_stream);
       
        id_type id;
        p_archive & id;
     
        // make the call
        if (options.marshal_option >= call_options::return_only)
        {
            BOOST_ASSERT(result);
            std::stringstream r_stream(std::ios::in | std::ios::out | std::ios::binary);
                typename archive_type::oarchive_type r_archive(r_stream);
            reg[id].call(options, p_archive, &r_archive);
            *result = r_stream.str();
        }
        else
            reg[id].call(options, p_archive, NULL);
    }
private:
    /// Registered functions.
    boost::ptr_map<Id, detail::function<ArchivePair> > reg;
};

} // namespace rpc
} // namespace boost

#endif // BOOST_RPC_REGISTRY_HPP
