// Copyright Stjepan Rajko 2007,2008. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_DATAFLOW_UTILITY_BIND_MEM_FN_OVERLOAD_HPP
#define BOOST_DATAFLOW_UTILITY_BIND_MEM_FN_OVERLOAD_HPP

#include <boost/dataflow/utility/bind_mem_fn.hpp>
#include <boost/dataflow/utility/slot_type.hpp>

namespace boost {  namespace dataflow {

namespace utility {

/// Binds a class member function of a specified Signature to a class object.
/** \returns boost::function type with the bound member function.
*/
template<typename Signature, typename T>
boost::function<Signature> bind_mem_fn_overload(typename dataflow::utility::slot_type<Signature, T>::type func, T &object)
{
    typedef typename boost::dataflow::utility::slot_type<Signature, T>::type mem_fn_type;

	return boost::dataflow::utility::bind_mem_fn<mem_fn_type, T>
            (static_cast<mem_fn_type>(func), object);
}

/// Binds a class const member function of a specified Signature to a non-const class object.
/** \returns boost::function type with the bound member function.
*/
template<typename Signature, typename T>
boost::function<Signature> bind_mem_fn_const_overload(typename dataflow::utility::slot_type<Signature, const T>::type func, T &object)
{
    typedef typename boost::dataflow::utility::slot_type<Signature, const T>::type mem_fn_type;

	return boost::dataflow::utility::bind_mem_fn<mem_fn_type, T>
            (static_cast<mem_fn_type>(func), object);
}
        
} // namespace utility

} } // namespace boost::dataflow

#endif // BOOST_DATAFLOW_UTILITY_BIND_MEM_FN_OVERLOAD_HPP
