// Copyright Stjepan Rajko 2008. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_DATAFLOW_UTILITY_MEMBER_FUNCTION_SIGNATURE_HPP
#define BOOST_DATAFLOW_UTILITY_MEMBER_FUNCTION_SIGNATURE_HPP

#include <boost/function_types/function_type.hpp>
#include <boost/function_types/is_member_function_pointer.hpp>
#include <boost/function_types/components.hpp>

#include <boost/mpl/assert.hpp>
#include <boost/mpl/erase.hpp>

namespace boost { namespace dataflow { namespace utility {

/** Removes the class from a member function pointer type, yielding the
    pure function signature (e.g., usable by boost::function).
*/
template<typename MemFn>
struct member_function_signature
{
    BOOST_MPL_ASSERT((function_types::is_member_function_pointer<MemFn> ));
    
    typedef
        typename function_types::function_type<
            typename mpl::erase<
                typename boost::function_types::components<MemFn>::type,
                typename mpl::next<
                    typename mpl::begin<
                        typename boost::function_types::components<MemFn>::type
                    >::type
                >::type
            >::type
        >::type type;
};

} } } // namespace boost::dataflow::utility

#endif // BOOST_DATAFLOW_UTILITY_MEMBER_FUNCTION_SIGNATURE_HPP
