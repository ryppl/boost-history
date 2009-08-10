//
// chat_defs.hpp
//
// Copyright (c) 2005-2009 Yigong Liu (yigongliu at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef CHAT_DEFS_HPP
#define CHAT_DEFS_HPP

#include <string>
#include <boost/tuple/tuple_comparison.hpp>
#include <boost/tuple/tuple_io.hpp>
#include <boost/channel/channel.hpp>

using namespace std;
using namespace boost::channel;

//instantiate channel type, define id_type
/**
 *  using the following tuple fields to identify a person
 *  string - first name
 *  string - last name
 *  int    - male(0), female(1)
 *  int    - department: math(0), physics(1), art(2)...
 */
typedef boost::tuple<string, string, int, int> tuple_type;
typedef tuple_id<tuple_type> id_type;

typedef channel<
  id_type,
  boost_platform,
  mt_synch<boost_platform>,
  abstract_executor, //force in place execution
  linear_name_space<id_type, abstract_executor, mt_synch<boost_platform>, false> ///false - assoc matching
///broadcast dispatcher by default
  > chat_chan;

//sample subject ids in chat_chan
//1. all msgs for student with last name "smith"
id_type smith_r_id(tuple_type(
			    field_trait<string>::wildcard,
			    "smith",
			    field_trait<int>::wildcard,
			    field_trait<int>::wildcard));
//2. all msgs for student in "math" department
id_type math_r_id(tuple_type(
			    field_trait<string>::wildcard,
			    field_trait<string>::wildcard,
			    field_trait<int>::wildcard,
			    0));
//2. all msgs for female
id_type female_r_id(tuple_type(
			    field_trait<string>::wildcard,
			    field_trait<string>::wildcard,
			    1,
			    field_trait<int>::wildcard));
//3. id for a specific person
id_type robin_s_id(tuple_type(
			    "robin",
			    "smith",
			    0,
			    2));
//4. id for a specific person
id_type adam_s_id(tuple_type(
			    "adam",
			    "smith",
			    0,
			    0));
//5. id for a specific person
id_type tina_s_id(tuple_type(
			    "tina",
			    "su",
			    1,
			    1));

//a simple struct for chat msg
struct chat_msg {
  string source_;
  string data_;
  chat_msg(const char *s, char *d) : source_(s), data_(d) {}
  chat_msg() {} //have to define this for marshaling to work
  //serialization function for chat_msg
  template <typename Archive>
  void serialize(Archive & ar, const unsigned int version)
  {
    ar &source_ & data_;
  }
};

//define name space system ids
DEFINE_ASSOC_SYS_IDS(tuple_type)

#endif
