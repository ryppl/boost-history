/*=================================---------------------------------------------
    Copyright 2007,2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/

#ifndef BOOST__DATAFLOW__UTILITY__ENABLE_IF_TYPE_HPP
#define BOOST__DATAFLOW__UTILITY__ENABLE_IF_TYPE_HPP


namespace boost { namespace dataflow {
    
namespace utility
{

    template<typename T, typename Result=void>
    struct enable_if_type
    {
        typedef Result type;
    };
    
    struct void_;
    
    template<typename T0=void_, typename T1=void_, typename T2=void_,
        typename T3=void_, typename T4=void_, typename T5=void_>
    struct all_of
    {
    };
    
} // namespace utility

} } // namespace boost::dataflow

#endif // BOOST__DATAFLOW__UTILITY__ENABLE_IF_TYPE_HPP
