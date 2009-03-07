//
// assoc_id_trait.cpp
//
// Copyright (c) 2005-2009 Yigong Liu (yigongliu at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#define BOOST_CHANNEL_SOURCE

#include <string>
#include <boost/channel/name_spaces/assoc_id_trait.hpp>
using namespace boost::channel;

/// definitions of system messages for regex_id ids
regex_id id_trait<regex_id>::channel_conn_msg = "_channel_conn_msg_";
regex_id id_trait<regex_id>::channel_disconn_msg = "_channel_disconn_msg_";
regex_id id_trait<regex_id>::init_subscription_info_msg = "_init_subscription_info_msg_";
regex_id id_trait<regex_id>::connection_ready_msg = "_connection_ready_msg_";
regex_id id_trait<regex_id>::subscription_info_msg = "_subscription_info_msg_";
regex_id id_trait<regex_id>::unsubscription_info_msg = "_unsubscription_info_msg_";
regex_id id_trait<regex_id>::publication_info_msg = "_publication_info_msg_";
regex_id id_trait<regex_id>::unpublication_info_msg = "_unpublication_info_msg_";

/// definitions of system messages for tuple_id ids
int field_trait<int>::wildcard = -1010101;  //should choose a better value
float field_trait<float>::wildcard = -1010101.0101;  //should choose a better value
const char * field_trait<std::string>::wildcard = "-1010101";  //should choose a better value
