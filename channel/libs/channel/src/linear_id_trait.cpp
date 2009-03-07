//
// linear_id_trait.cpp
//
// Copyright (c) 2005-2009 Yigong Liu (yigongliu at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#define BOOST_CHANNEL_SOURCE

#include <boost/channel/name_spaces/linear_id_trait.hpp>
#include <string>

using namespace std;
using namespace boost::channel;

null_id id_trait<null_id>::channel_conn_msg = null_id();
null_id id_trait<null_id>::channel_disconn_msg = null_id();
null_id id_trait<null_id>::init_subscription_info_msg = null_id();
null_id id_trait<null_id>::connection_ready_msg = null_id();
null_id id_trait<null_id>::subscription_info_msg = null_id();
null_id id_trait<null_id>::unsubscription_info_msg = null_id();
null_id id_trait<null_id>::publication_info_msg = null_id();
null_id id_trait<null_id>::unpublication_info_msg = null_id();
null_id id_trait<null_id>::app_msg_begin = null_id();

///--- sys msg starts at 1000 ---
int id_trait<int>::channel_conn_msg=1000;
int id_trait<int>::channel_disconn_msg=1001;
int id_trait<int>::init_subscription_info_msg=1002;
int id_trait<int>::connection_ready_msg=1003;
int id_trait<int>::subscription_info_msg=1004;
int id_trait<int>::unsubscription_info_msg=1005;
int id_trait<int>::publication_info_msg=1006;
int id_trait<int>::unpublication_info_msg=1007;
///--- app msg starts at 2000 ---
int id_trait<int>::app_msg_begin=2000;

/// definitions of system messages for string ids
string id_trait<string>::channel_conn_msg = "_channel_conn_msg_";
string id_trait<string>::channel_disconn_msg = "_channel_disconn_msg_";
string id_trait<string>::init_subscription_info_msg = "_init_subscription_info_msg_";
string id_trait<string>::connection_ready_msg = "_connection_ready_msg_";
string id_trait<string>::subscription_info_msg = "_subscription_info_msg_";
string id_trait<string>::unsubscription_info_msg = "_unsubscription_info_msg_";
string id_trait<string>::publication_info_msg = "_publication_info_msg_";
string id_trait<string>::unpublication_info_msg = "_unpublication_info_msg_";

/// definitions of system messages for struct ids
struct_id id_trait<struct_id>::channel_conn_msg = {system_message, 1};
struct_id id_trait<struct_id>::channel_disconn_msg = {system_message, 2};
struct_id id_trait<struct_id>::init_subscription_info_msg = {system_message, 3};
struct_id id_trait<struct_id>::connection_ready_msg = {system_message, 4};
struct_id id_trait<struct_id>::subscription_info_msg = {system_message, 5};
struct_id id_trait<struct_id>::unsubscription_info_msg = {system_message, 6};
struct_id id_trait<struct_id>::publication_info_msg = {system_message, 7};
struct_id id_trait<struct_id>::unpublication_info_msg = {system_message, 8};
