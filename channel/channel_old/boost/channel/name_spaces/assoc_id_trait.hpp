//
// assoc_id_trait.hpp
//
// Copyright (c) 2005-2009 Yigong Liu (yigongliu at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef ASSOC_ID_TRAIT_HPP
#define ASSOC_ID_TRAIT_HPP

#include <iostream>
#include <string>
#include <sstream>
#include <boost/regex.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/split_member.hpp>
#include <boost/channel/config.hpp>
#include <boost/tuple/tuple_comparison.hpp>
#include <boost/tuple/tuple_io.hpp>

namespace boost {
  namespace channel {

    /**
     *     regex_id type class
     *     using strings/regex as id and use regex-matching for id matching
     */
    struct BOOST_CHANNEL_DECL regex_id {
      bool is_regex_;
      std::string text_;
      boost::shared_ptr<boost::regex> re_;
      regex_id() :
        is_regex_(false), text_(""), re_() {
      }
      regex_id(const char * id_text) :
        is_regex_(false), text_(id_text), re_() {
      }
      regex_id(boost::regex *reg) :
        is_regex_(true), text_(""), re_(reg) {
      }
      bool operator<(const regex_id &id) const {
        if (!is_regex_ && id.is_regex_)
          return true;
        else if (!is_regex_ && !id.is_regex_ && text_ < id.text_)
          return true;
        else if (is_regex_ && id.is_regex_ && (*re_) < (*id.re_))
          return true;
        return false;
      }
      bool operator==(const regex_id &id) const {
        if (!is_regex_ && !id.is_regex_ && text_ == id.text_)
          return true;
        else if (is_regex_ && id.is_regex_ && (*re_) == (*id.re_))
          return true;
        return false;
      }
      bool operator!=(const regex_id &id) const {
        return !operator==(id);
      }
      template<class Archive>
      void save(Archive & ar, const unsigned int version) const {
        ar & is_regex_;
        if (!is_regex_) {
          ar & text_;
          //std::cout << "save id: " << text_ << std::endl;
        } else {
          typename boost::regex::flag_type f = re_->flags();
          std::string str = re_->str();
          ar & f;
          ar & str;
          //std::cout << "save id: " << *re_ << std::endl;
        }
      }
      template<class Archive>
      void load(Archive & ar, const unsigned int version) {
        ar & is_regex_;
        if (!is_regex_) {
          ar & text_;
          //std::cout << "load id: " << text_ << std::endl;
        } else {
          typename boost::regex::flag_type f;
          std::string str;
          ar & f;
          ar & str;
          re_.reset(new boost::regex(str, f));
          //std::cout << "load id: " <<* re_ << std::endl;
        }
      }
      BOOST_SERIALIZATION_SPLIT_MEMBER()
    };

    /**
     *     regex_id_trait class
     */
    template<class id_type> class id_trait;

    template <>
    class BOOST_CHANNEL_DECL id_trait<regex_id> {
    public:
      ///define system msgs
      static regex_id channel_conn_msg;
      static regex_id channel_disconn_msg;
      static regex_id init_subscription_info_msg;
      static regex_id connection_ready_msg;
      static regex_id subscription_info_msg;
      static regex_id unsubscription_info_msg;
      static regex_id publication_info_msg;
      static regex_id unpublication_info_msg;

      typedef regex_id id_type;

      static bool match(regex_id id1, regex_id id2)
      {
        //std::cout << "try to match : " << id_to_string(id1) << "; " << id_to_string(id2) << std::endl;
        if (!id1.is_regex_ && !id2.is_regex_)
          return id1.text_ == id2.text_;
        else if (id1.is_regex_ && !id2.is_regex_)
          return boost::regex_match(id2.text_, *(id1.re_));
        else if (id2.is_regex_ && !id1.is_regex_)
          return boost::regex_match(id1.text_, *(id2.re_));
        else if (id1.is_regex_ && id2.is_regex_)
          return (*id1.re_) == (*id2.re_);
        return false;
      }

      //are we using assoc/fuzzy matching here
      static bool wildcard_name(const regex_id &id) {
        return id.is_regex_;
      }

      static std::string id_to_string(const regex_id &id) {
        std::ostringstream os;
        if (id.is_regex_)
          os << "regex_id " << "[regex_pattern]: " << *(id.re_);
        else
          os << "regex_id " << "[plain_text]: " << id.text_;
        return os.str();
      }
    };

    /**
     *  tuple_id : use linda style tuple as id and associative lookup for matching
     *  a tuple_id consist of multiple fields
     *  we should just define field_trait for primitive types here
     *  user should define field_traits for user-defined types
     */
    /**
     *  basic field_trait
     */
    template<typename field_type> class field_trait;

    template <>
    class BOOST_CHANNEL_DECL field_trait<int> {
    public:
      typedef int field_type;
      static int wildcard;
      static bool wildcard_field(int f) {
        if (f == wildcard)
          return true;
        return false;
      }
      static bool match(int f1, int f2) {
        if (f1 == wildcard || f2 == wildcard)
          return true;
        return f1 == f2;
      }
    };

    template <>
    class BOOST_CHANNEL_DECL field_trait<float> {
    public:
      typedef float field_type;
      static float wildcard;
      static bool wildcard_field(float f) {
        if (f == wildcard)
          return true;
        return false;
      }
      static bool match(float f1, float f2) {
        if (f1 == wildcard || f2 == wildcard)
          return true;
        return f1 == f2;
      }
    };

    template <>
    class BOOST_CHANNEL_DECL field_trait<std::string> {
    public:
      typedef std::string field_type;
      static const char * wildcard;
      static bool wildcard_field(std::string f) {
        if (f == wildcard)
          return true;
        return false;
      }
      static bool match(std::string f1, std::string f2) {
        if (f1 == wildcard || f2 == wildcard)
          return true;
        return f1 == f2;
      }
    };

    /* --- we should add field_trait for other primitive types here --- */

    /**
     * use boost::tuple as tuple_id
     */
    template<typename Tuple_Type>
    class tuple_id {
    public:
      typedef Tuple_Type tuple_type;
      enum sys_ids {
        app_type = 0,
        channel_conn_type,
        channel_disconn_type,
        init_subscription_info_type,
        connection_ready_type,
        subscription_info_type,
        unsubscription_info_type,
        publication_info_type,
        unpublication_info_type
      };
      int type_;
      tuple_type tuple_;
      tuple_id() :
        type_(app_type), tuple_() {
      }
      tuple_id(int type) :
        type_(type), tuple_() {
      }
      tuple_id(tuple_type tt) :
        type_(app_type), tuple_(tt) {
      }
      tuple_id(const tuple_id &id) :
        type_(id.type_), tuple_(id.tuple_) {
      }
      bool operator<(const tuple_id &id) const {
        if (type_ < id.type_)
          return true;
        else if (type_ == id.type_ && type_ == app_type && tuple_ < id.tuple_)
          return true;
        return false;
      }
      bool operator==(const tuple_id &id) const {
        if (type_ == id.type_) {
          if (type_ != app_type)
            return true;
          else
            return tuple_ == id.tuple_;
        }
        return false;
      }
      bool operator!=(const tuple_id &id) const {
        return !operator==(id);
      }
      template<class Archive>
      inline void serialize_priv(Archive & ar, const boost::tuples::null_type&,
                                 const unsigned int version) {
      }
      template<class Archive, typename H, typename T>
      inline void serialize_priv(Archive & ar, boost::tuples::cons<H, T> &t,
                                 const unsigned int version) {
        ar & t.get_head();
        serialize_priv(ar, t.get_tail(), version);
      }

      template<class Archive>
      void serialize(Archive & ar, const unsigned int version) {
        ar & type_;
        if (type_ == app_type) {
          ar & tuple_.get_head();
          serialize_priv(ar, tuple_.get_tail(), version);
        }
        //std::cout << "serialize tuple_id : type[" << type_ << "] tuple[" << tuple_ << "]" << std::endl;
      }
    };

    /**
     * tuple id trait
     */
    template<typename Tuple_Type>
    class BOOST_CHANNEL_DECL id_trait<tuple_id<Tuple_Type> > {
    public:

      typedef Tuple_Type tuple_type;
      typedef tuple_id<tuple_type> id_type;

      ///define system msgs
      static id_type channel_conn_msg;
      static id_type channel_disconn_msg;
      static id_type init_subscription_info_msg;
      static id_type connection_ready_msg;
      static id_type subscription_info_msg;
      static id_type unsubscription_info_msg;
      static id_type publication_info_msg;
      static id_type unpublication_info_msg;

      static bool match_tuple(const boost::tuples::null_type&, const boost::tuples::null_type&)
      {
        //std::cout << "match_tuple(const boost::tuples::null_type  matched tuple \n";
        return true;
      }

      template <typename H, typename T>
	static bool match_tuple(const boost::tuples::cons<H,T> &t1, const boost::tuples::cons<H,T> &t2)
      {
        if(!field_trait<H>::match(t1.get_head(), t2.get_head()))
          return false;
        return match_tuple(t1.get_tail(), t2.get_tail());
      }

      static bool match(id_type id1, id_type id2)
      {
        if (id1.type_ != id2.type_)
          return false;
        if (id1.type_ == id_type::app_type) {
          if (wildcard_tuple(id1.tuple_) && wildcard_tuple(id2.tuple_) && (id1.tuple_ != id2.tuple_))
            return false;
          if (id1.tuple_ == id2.tuple_)
            return true;
          return match_tuple(id1.tuple_, id2.tuple_);
        }
        return true;
      }

      //are we using assoc/fuzzy matching here
      static bool wildcard_tuple(const boost::tuples::null_type&)
      {
        return false;
      }

      template <typename H, typename T>
	static bool wildcard_tuple(const boost::tuples::cons<H,T> &t1)
      {
        if(field_trait<H>::wildcard_field(t1.get_head()))
          return true;
        return wildcard_tuple(t1.get_tail());
      }

      static bool wildcard_name(const id_type &id) {
        if (id.type_ != id_type::app_type)
          return false;
        return wildcard_tuple(id.tuple_);
      }

      static std::string id_to_string(const id_type &id) {
        std::ostringstream os;
        os << "tuple_id : type[" << id.type_ << "] tuple[" << id.tuple_ << "]";
        return os.str();
      }
    };

  }
}

#define DEFINE_ASSOC_SYS_IDS( tuple_type )                              \
  template <> boost::channel::tuple_id<tuple_type> boost::channel::id_trait<boost::channel::tuple_id<tuple_type> >::channel_conn_msg(boost::channel::tuple_id<tuple_type>::channel_conn_type); \
  template <> boost::channel::tuple_id<tuple_type> boost::channel::id_trait<boost::channel::tuple_id<tuple_type> >::channel_disconn_msg(boost::channel::tuple_id<tuple_type>::channel_disconn_type); \
  template <> boost::channel::tuple_id<tuple_type> boost::channel::id_trait<boost::channel::tuple_id<tuple_type> >::init_subscription_info_msg(boost::channel::tuple_id<tuple_type>::init_subscription_info_type); \
  template <> boost::channel::tuple_id<tuple_type> boost::channel::id_trait<boost::channel::tuple_id<tuple_type> >::connection_ready_msg(boost::channel::tuple_id<tuple_type>::connection_ready_type); \
  template <> boost::channel::tuple_id<tuple_type> boost::channel::id_trait<boost::channel::tuple_id<tuple_type> >::subscription_info_msg(boost::channel::tuple_id<tuple_type>::subscription_info_type); \
  template <> boost::channel::tuple_id<tuple_type> boost::channel::id_trait<boost::channel::tuple_id<tuple_type> >::unsubscription_info_msg(boost::channel::tuple_id<tuple_type>::unsubscription_info_type); \
  template <> boost::channel::tuple_id<tuple_type> boost::channel::id_trait<boost::channel::tuple_id<tuple_type> >::publication_info_msg(boost::channel::tuple_id<tuple_type>::publication_info_type); \
  template <> boost::channel::tuple_id<tuple_type> boost::channel::id_trait<boost::channel::tuple_id<tuple_type> >::unpublication_info_msg(boost::channel::tuple_id<tuple_type>::unpublication_info_type);

#endif

