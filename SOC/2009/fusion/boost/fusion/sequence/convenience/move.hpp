/*=============================================================================
    Copyright (c) 2010 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_SEQUENCE_CONVENIENCE_MOVE_HPP
#define BOOST_FUSION_SEQUENCE_CONVENIENCE_MOVE_HPP

#include <boost/config.hpp>
#ifdef BOOST_NO_RVALUE_REFERENCES
#   error "Your compiler must support rvalue reference in order to support moving."
#endif

namespace boost { namespace fusion
{
    namespace detail
    {
        struct move
        {
            template<class ZippedElements>
            void
            operator()(ZippedElements const& zipped_elements)const
            {
                fusion::back(zipped_elements)=
                    std::move(fusion::front(zipped_elements));
            }
        };
    }

    namespace result_of
    {
        template<typename From, typename To>
        struct move
        {
            BOOST_FUSION_MPL_ASSERT((traits::is_sequence<From>));
            BOOST_FUSION_MPL_ASSERT((traits::is_sequence<To>));

            typedef void type;
        };
    }

    template<typename From, typename To>
    typename result_of::move<From&,To&>::type
    move(From& from,To& to)
    {
        fusion::for_each(fusion::zip(from,to),detail::mover());
    }
}}

#endif
