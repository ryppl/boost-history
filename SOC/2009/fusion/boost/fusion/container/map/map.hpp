/*=============================================================================
    Copyright (c) 2005 Joel de Guzman
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_CONTAINER_MAP_MAP_HPP
#define BOOST_FUSION_CONTAINER_MAP_MAP_HPP

#include <boost/fusion/container/map/map_fwd.hpp>
#include <boost/fusion/container/vector/vector.hpp>
#include <boost/fusion/support/pair.hpp>
#include <boost/fusion/support/sequence_base.hpp>
#include <boost/fusion/support/internal/ref.hpp>

#include <boost/mpl/bool.hpp>

#include <boost/fusion/container/map/detail/begin_impl.hpp>
#include <boost/fusion/container/map/detail/end_impl.hpp>
#include <boost/fusion/container/map/detail/deref_impl.hpp>
#include <boost/fusion/container/map/detail/value_of_impl.hpp>
#include <boost/fusion/container/map/detail/value_of_data_impl.hpp>
#include <boost/fusion/container/map/detail/deref_data_impl.hpp>
#include <boost/fusion/container/map/detail/key_of_impl.hpp>

#include <boost/fusion/container/detail/forward_interface.hpp>

namespace boost { namespace fusion
{
    struct fusion_sequence_tag;

    VARIADIC_TEMPLATE(FUSION_MAX_MAP_SIZE)
    struct map
      : sequence_base<map<EXPAND_TEMPLATE_ARGUMENTS(FUSION_MAX_MAP_SIZE)> >
    {
        typedef map_category category;
        typedef map_tag fusion_tag;
        typedef fusion_sequence_tag tag; 
        typedef mpl::false_ is_view;

        typedef
            vector<EXPAND_TEMPLATE_ARGUMENTS(FUSION_MAX_MAP_SIZE)>
        storage_type;
        typedef typename storage_type::size size;

#define BOOST_FUSION_SEQ_NAME map
#define BOOST_FUSION_MAX_SEQ_SIZE FUSION_MAX_MAP_SIZE
#include <boost/fusion/container/detail/forward_interface.hpp>
#undef BOOST_FUSION_MAX_SEQ_SIZE
#undef BOOST_FUSION_SEQ_NAME

        storage_type& get_data() { return data; }
        storage_type const& get_data() const { return data; }

    private:
        storage_type data;
    };
}}

#endif
