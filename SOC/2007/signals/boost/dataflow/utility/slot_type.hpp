// Copyright Stjepan Rajko 2007,2008. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_DATAFLOW_UTILITY_SLOT_TYPE_HPP
#define BOOST_DATAFLOW_UTILITY_SLOT_TYPE_HPP

#include <boost/function_types/member_function_pointer.hpp>
#include <boost/function_types/parameter_types.hpp>
#include <boost/function_types/result_type.hpp>
#include <boost/mpl/push_front.hpp>

namespace boost {  namespace dataflow {

namespace utility {

/// Constructs a type corresponding to pointer to member of T with signature Signature.
/** e.g. slot_type<some_class, void(float)>::type is void (some_class::*) (float)
*/
    template <typename Signature, typename T>
    struct slot_type
    {
        typedef
            typename boost::function_types::member_function_pointer<
                typename boost::mpl::push_front<
                    typename boost::mpl::push_front<
                        typename boost::function_types::parameter_types<Signature>::type, T
                    >::type, typename boost::function_types::result_type<Signature>::type
                >::type
            >::type type;
    };
        
} // namespace utility

} } // namespace boost::dataflow

#endif // BOOST_DATAFLOW_UTILITY_SLOT_TYPE_HPP
