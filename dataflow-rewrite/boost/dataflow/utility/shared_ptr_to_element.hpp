/*=================================---------------------------------------------
    Copyright 2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/

#ifndef BOOST__DATAFLOW__UTILITY__SHARED_PTR_TO_ELEMENT_HPP
#define BOOST__DATAFLOW__UTILITY__SHARED_PTR_TO_ELEMENT_HPP


#include <boost/shared_ptr.hpp>
#include <boost/fusion/include/value_at.hpp>
#include <boost/fusion/include/at.hpp>


namespace boost { namespace dataflow {
    
namespace utility
{    
    namespace result_of
    {
        template<typename Sequence, int N>
        struct shared_ptr_to_element_c
        {
            typedef 
                boost::shared_ptr
                <
                    typename fusion::result_of::value_at_c<Sequence, N>::type
                >
                    type;
        };

        template<typename Sequence, typename N>
        struct shared_ptr_to_element
        {
            typedef
                typename shared_ptr_to_element_c<Sequence, N::value>::type
                    type;
        };
    }
    
    template<int N, typename Sequence>
    typename result_of::shared_ptr_to_element_c<Sequence, N>::type
        shared_ptr_to_element_c(const boost::shared_ptr<Sequence> &ptr)
    {
        return
            typename result_of::shared_ptr_to_element_c<Sequence, N>::type
            (
                ptr,
                &fusion::at_c<N>(*ptr)
            );
    }
    
    template<typename N, typename Sequence>
    typename result_of::shared_ptr_to_element<Sequence, N>::type
        shared_ptr_to_element(const boost::shared_ptr<Sequence> &ptr)
    {
        return
            typename result_of::shared_ptr_to_element<Sequence, N>::type
            (
                ptr,
                &fusion::at<N>(*ptr)
            );
    }
    
} // namespace utility

} } // namespace boost::dataflow

#endif // BOOST__DATAFLOW__UTILITY__ALL_OF_HPP
