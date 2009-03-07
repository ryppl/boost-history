//
// hierarchical_id_trait.cpp
//
// Copyright (c) 2005-2009 Yigong Liu (yigongliu at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#define BOOST_CHANNEL_SOURCE

#include <boost/channel/name_spaces/hierarchical_id_trait.hpp>
#include <string>

using namespace std;
using namespace boost::channel;

/// definitions of system messages for str_path_id<'/'> ids
template <> str_path_id<'/'> id_trait<str_path_id<'/'> >::channel_conn_msg = "/sys/channel_conn_msg";
template <> str_path_id<'/'> id_trait<str_path_id<'/'> >::channel_disconn_msg = "/sys/channel_disconn_msg";
template <> str_path_id<'/'> id_trait<str_path_id<'/'> >::init_subscription_info_msg = "/sys/init_subscription_info_msg";
template <> str_path_id<'/'> id_trait<str_path_id<'/'> >::connection_ready_msg = "/sys/connection_ready_msg";
template <> str_path_id<'/'> id_trait<str_path_id<'/'> >::subscription_info_msg = "/sys/subscription_info_msg";
template <> str_path_id<'/'> id_trait<str_path_id<'/'> >::unsubscription_info_msg = "/sys/unsubscription_info_msg";
template <> str_path_id<'/'> id_trait<str_path_id<'/'> >::publication_info_msg = "/sys/publication_info_msg";
template <> str_path_id<'/'> id_trait<str_path_id<'/'> >::unpublication_info_msg = "/sys/unpublication_info_msg";
template <> str_path_id<'/'>::value_type id_trait<str_path_id<'/'> >::root_token = "//";
template <> str_path_id<'/'>::value_type id_trait<str_path_id<'/'> >::wildcard_token = "*";

/// definitions of system messages for str_path_id<'|'> ids
template <> str_path_id<'|'> id_trait<str_path_id<'|'> >::channel_conn_msg = "|sys|channel_conn_msg";
template <> str_path_id<'|'> id_trait<str_path_id<'|'> >::channel_disconn_msg = "|sys|channel_disconn_msg";
template <> str_path_id<'|'> id_trait<str_path_id<'|'> >::init_subscription_info_msg = "|sys|init_subscription_info_msg";
template <> str_path_id<'|'> id_trait<str_path_id<'|'> >::connection_ready_msg = "|sys|connection_ready_msg";
template <> str_path_id<'|'> id_trait<str_path_id<'|'> >::subscription_info_msg = "|sys|subscription_info_msg";
template <> str_path_id<'|'> id_trait<str_path_id<'|'> >::unsubscription_info_msg = "|sys|unsubscription_info_msg";
template <> str_path_id<'|'> id_trait<str_path_id<'|'> >::publication_info_msg = "|sys|publication_info_msg";
template <> str_path_id<'|'> id_trait<str_path_id<'|'> >::unpublication_info_msg = "|sys|unpublication_info_msg";
template <> str_path_id<'|'>::value_type id_trait<str_path_id<'|'> >::root_token = "||";
template <> str_path_id<'|'>::value_type id_trait<str_path_id<'|'> >::wildcard_token = "*";

/// definitions of system messages for str_path_id<'\'> ids
template <> str_path_id<'\\'> id_trait<str_path_id<'\\'> >::channel_conn_msg = "\\sys\\channel_conn_msg";
template <> str_path_id<'\\'> id_trait<str_path_id<'\\'> >::channel_disconn_msg = "\\sys\\channel_disconn_msg";
template <> str_path_id<'\\'> id_trait<str_path_id<'\\'> >::init_subscription_info_msg = "\\sys\\init_subscription_info_msg";
template <> str_path_id<'\\'> id_trait<str_path_id<'\\'> >::connection_ready_msg = "\\sys\\connection_ready_msg";
template <> str_path_id<'\\'> id_trait<str_path_id<'\\'> >::subscription_info_msg = "\\sys\\subscription_info_msg";
template <> str_path_id<'\\'> id_trait<str_path_id<'\\'> >::unsubscription_info_msg = "\\sys\\unsubscription_info_msg";
template <> str_path_id<'\\'> id_trait<str_path_id<'\\'> >::publication_info_msg = "\\sys\\publication_info_msg";
template <> str_path_id<'\\'> id_trait<str_path_id<'\\'> >::unpublication_info_msg = "\\sys\\unpublication_info_msg";
template <> str_path_id<'\\'>::value_type id_trait<str_path_id<'\\'> >::root_token = "\\\\";
template <> str_path_id<'\\'>::value_type id_trait<str_path_id<'\\'> >::wildcard_token = "*";



