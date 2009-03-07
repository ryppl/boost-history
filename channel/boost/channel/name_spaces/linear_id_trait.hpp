//
// linear_id_trait.hpp
//
// Copyright (c) 2005-2009 Yigong Liu (yigongliu at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//


#ifndef LINEAR_ID_TRAIT_HPP
#define LINEAR_ID_TRAIT_HPP

#include <sstream>
#include <string>
#include <boost/channel/config.hpp>

/**
 * Here are the definitions related to linear name_space ids:
 * 1> trait class for primitive id types: int, string,...
 * 2> definitions of non-primitive id types: struct id,... and their traits
 */

namespace boost {
  namespace channel {

    ///no default definition for id trait; must explicitly define it for each id type
    template<class id_type> class id_trait;

    /**
     *  define null_id to avoid overhead for local port and signal
     */
    class null_id {
    public:
      bool operator==(const null_id &) const {
        return false;
      }
      bool operator<(const null_id &) const {
        return false;
      }
      bool operator!=(const null_id &) const {
        return false;
      }
    };

    /// trait class for null ids
    template <>
    class BOOST_CHANNEL_DECL id_trait<null_id> {
    public:
      ///define system msgs
      static null_id channel_conn_msg;
      static null_id channel_disconn_msg;
      static null_id init_subscription_info_msg;
      static null_id connection_ready_msg;
      static null_id subscription_info_msg;
      static null_id unsubscription_info_msg;
      static null_id publication_info_msg;
      static null_id unpublication_info_msg;
      static null_id app_msg_begin;

      typedef null_id id_type;

      ///need the comparisons for msg matching and map
      static bool eq(const id_type &id1, const id_type &id2)
      {	return false;}
      static bool lt(const id_type &id1, const id_type &id2)
      {	return false;}

      //compiler doesnt allow references here
      //static bool match(const id_type &id1, const id_type &id2)
      static bool match(id_type id1, id_type id2)
      {	return false;}

      //linear name_space dont support wildcard yet
      static bool wildcard_name(const id_type &id) {
        return false;
      }

      ///string representation of ids, for debugging
      static std::string id_to_string(const id_type &id) {
        return "null_id";
      }

    };

    /// trait class for integer ids
    template <>
    class BOOST_CHANNEL_DECL id_trait<int> {
    public:
      ///define system msgs
      /*
        enum sys_ids {
        ///--- sys msg starts at 1000 ---
        channel_conn_msg=1000,
        channel_disconn_msg,
        init_subscription_info_msg,
        connection_ready_msg,
        subscription_info_msg,
        unsubscription_info_msg,
        publication_info_msg,
        unpublication_info_msg,
        ///--- app msg starts at 2000 ---
        app_msg_begin=2000
        };
      */
      static int channel_conn_msg;
      static int channel_disconn_msg;
      static int init_subscription_info_msg;
      static int connection_ready_msg;
      static int subscription_info_msg;
      static int unsubscription_info_msg;
      static int publication_info_msg;
      static int unpublication_info_msg;
      static int app_msg_begin;

      typedef int id_type;

      ///need the comparisons for msg matching and map
      static bool eq(const id_type &id1, const id_type &id2)
      {	return id1 == id2;}
      static bool lt(const id_type &id1, const id_type &id2)
      {	return id1 < id2;}

      //compiler doesnt allow references here
      //static bool match(const id_type &id1, const id_type &id2)
      static bool match(id_type id1, id_type id2)
      {	return id1 == id2;}

      //linear name_space dont support wildcard yet
      static bool wildcard_name(const id_type &id) {
        return false;
      }

      ///string representation of ids, for debugging
      static std::string id_to_string(const id_type &id) {
        std::ostringstream os;
        os << id;
        return os.str();
      }

    };

    /// trait class for string ids
    template <>
    class BOOST_CHANNEL_DECL id_trait<std::string> {
    public:
      ///define system msgs

      static std::string channel_conn_msg;
      static std::string channel_disconn_msg;
      static std::string init_subscription_info_msg;
      static std::string connection_ready_msg;
      static std::string subscription_info_msg;
      static std::string unsubscription_info_msg;
      static std::string publication_info_msg;
      static std::string unpublication_info_msg;

      typedef std::string id_type;

      ///need the comparisons for msg matching and map
      static bool eq(const std::string &id1, const std::string &id2)
      {	return id1 == id2;}
      static bool lt(const std::string &id1, const std::string &id2)
      {	return id1 < id2;}

      //compiler doesnt allow references here
      //static bool match(const std::string &id1, const std::string &id2)
      static bool match(std::string id1, std::string id2)
      {	return id1 == id2;}

      //linear name_space dont support wildcard yet
      static bool wildcard_name(const id_type &id) {
        return false;
      }

      static std::string id_to_string(const std::string &id) {
        return id;
      }

    };

    /**
     * definition of sample POD struct ids
     */
    ///ids have 2 fields: family and type
    enum message_family {
      system_message, application_message
    };

    struct BOOST_CHANNEL_DECL struct_id {
      message_family family;
      int type;
      bool operator<(const struct_id &id) const {
        if (family < id.family || (family == id.family && type < id.type))
          return true;
        return false;
      }
      bool operator==(const struct_id &id) const {
        if (family == id.family && type == id.type)
          return true;
        return false;
      }
      bool operator!=(const struct_id &id) const {
        if (family != id.family || type != id.type)
          return true;
        return false;
      }
      ///serializer for struct_id as id_type
      template<class Archive>
      void serialize(Archive & ar, const unsigned int version) {
        ar & family & type;
      }

    };

    /// trait class for sample POD struct ids
    template<>
    class BOOST_CHANNEL_DECL id_trait<struct_id> {
    public:
      ///define system msgs

      static struct_id channel_conn_msg;
      static struct_id channel_disconn_msg;
      static struct_id init_subscription_info_msg;
      static struct_id connection_ready_msg;
      static struct_id subscription_info_msg;
      static struct_id unsubscription_info_msg;
      static struct_id publication_info_msg;
      static struct_id unpublication_info_msg;

      typedef struct_id id_type;

      ///need the comparisons for msg matching and map
      static bool eq(const struct_id &id1, const struct_id &id2)
      {	return id1 == id2;}
      static bool lt(const struct_id &id1, const struct_id &id2)
      {	return id1 < id2;}

      //compiler doesnt allow references here
      //static bool match(const struct_id &id1, const struct_id &id2)
      static bool match(struct_id id1, struct_id id2)
      {	return id1 == id2;}

      //linear name_space dont support wildcard yet
      static bool wildcard_name(const id_type &id) {
        return false;
      }

      static std::string id_to_string(const struct_id &id) {
        std::ostringstream os;
        os << "[Family:" << id.family << ", Type:" << id.type <<"]";
        return os.str();
      }
    };
  }
}

#endif
