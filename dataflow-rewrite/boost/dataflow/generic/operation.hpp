/*=================================---------------------------------------------
    Copyright 2007,2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/

#ifndef BOOST__DATAFLOW__GENERIC__OPERATION_HPP
#define BOOST__DATAFLOW__GENERIC__OPERATION_HPP


#include <boost/dataflow/generic/operation/semantics.hpp>
#include <boost/dataflow/utility/guarded_enable_if_type.hpp>
#include <boost/mpl/bool.hpp>


namespace boost { namespace dataflow {

template<typename Semantics>
struct operation
{
    typedef Semantics semantics;
};


/// Boolean Metafunction determining whether a type is a Operation.
template<typename Operation, typename Enable=utility::enable_guard>
struct is_operation : public mpl::false_
{
    /// INTERNAL ONLY
    BOOST_MPL_ASSERT(( is_same<Enable, utility::enable_guard> ));
};

/// INTERNAL ONLY
template<typename Operation>
struct is_operation<Operation,
    typename utility::guarded_enable_if_type<
        typename Operation::semantics
    >::type>
 : public mpl::true_
{};

} } // namespace boost::dataflow


#endif // BOOST__DATAFLOW__GENERIC__OPERATION_HPP
