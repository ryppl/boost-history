//
// hierarchical_id_trait.hpp
//
// Copyright (c) 2005-2009 Yigong Liu (yigongliu at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef HIERARCHICAL_ID_TRAIT_HPP
#define HIERARCHICAL_ID_TRAIT_HPP

#include <vector>
#include <string>
#include <sstream>
#include <boost/serialization/string.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/channel/config.hpp>

namespace boost {
  namespace channel {
    /**
     * hierarchical id types:
     * each id/name in hierarchical name_space is a "path name" of sub-ids/tokens
     * denoting the "branches" of tree you traverse thru before reaching
     * the tree-node which represents the id; these sub-ids called tokens
     * 1. if token_type is "char", HierarchicalId will be strings
     * 2. if token_type is "string", HierarichalId is unix style pathnames
     *
     * vector<token_type> is used as the base class for all hierarchical id
     * to store the elements of hierarchical ids
     * real applications' hierarchial ids should inherit vector class and
     * add :
     * 1. convenience constructors
     * 2. marshal/demarshal methods
     * 3. other convenience methods
     */

    template<class id_type> class id_trait;

    /**
     * str_path_id - a hierarchical id type using unix path names
     * element_type = string
     * in channel, only absolute pathnames are used: ie. must start with "/"
     * add convenience constructors and methods for decoding the names
     */
    template<char sep = '/'>
    struct BOOST_CHANNEL_DECL str_path_id: public std::vector<std::string> {
      enum {
        separator = sep
      };
      bool valid_;
      str_path_id() {
        valid_ = true;
      }
      str_path_id(const char * pathname) {
        std::string name(pathname);
        valid_ = true;
        size_type pos1 = std::string::npos, pos2 = std::string::npos;
        pos1 = name.find(separator);
        if (pos1 == std::string::npos) {
          valid_ = false;
        } else {
          while (true) {
            pos2 = name.find(separator, pos1 + 1);
            if (pos2 != std::string::npos)
              push_back(name.substr(pos1 + 1, pos2 - pos1 - 1));
            else {
              std::string s = name.substr(pos1 + 1);
              pos2 = s.find_last_not_of(' ');
              if (pos2 != std::string::npos)
                push_back(s.substr(0, pos2 + 1));
              break;
            }
            pos1 = pos2;
          }
        }
      }
      std::string get_str_name(void) {
        std::string name;
        for (size_t i = 0; i < size(); i++) {
          name.push_back(sep);
          name.append(operator[](i));
        }
        return name;
      }

      template<class Archive>
      void serialize(Archive & ar, const unsigned int version) {
        ar
          & boost::serialization::base_object<std::vector<std::string> >(
                                                                         *this);
      }
    };

    ///trait class for string pathname id type
    template<char sep>
    class BOOST_CHANNEL_DECL id_trait<str_path_id<sep> > {
    public:

      typedef str_path_id<sep> id_type;
      enum {separator = id_type::separator};

      typedef typename id_type::value_type token_type;

      ///define system msgs
      static id_type channel_conn_msg;
      static id_type channel_disconn_msg;
      static id_type init_subscription_info_msg;
      static id_type connection_ready_msg;
      static id_type subscription_info_msg;
      static id_type unsubscription_info_msg;
      static id_type publication_info_msg;
      static id_type unpublication_info_msg;

      static token_type root_token; //just a name for root trie node, not in name_space
      static token_type wildcard_token;

      ///need the comparisons for msg matching and map
      static bool eq(const id_type &id1, const id_type &id2)
      {	return id1 == id2;}
      static bool lt(const id_type &id1, const id_type &id2)
      {	return id1 < id2;}

      static std::string id_to_string(const id_type &id) {
        std::ostringstream os;
        for(size_t i=0; i<id.size(); i++)
          os << (char)separator << id[i];
        return os.str();
      }

      static bool wildcard_name(const id_type &id) {
        return (*id.rbegin()) == wildcard_token;
      }

      static bool valid(const id_type &id) {
        if (!id.valid_) return false;
        size_t sz = id.size();
        for (size_t i=0; i<sz; i++)
          if (id[i] == id_trait::wildcard_token && i != (sz-1))
            return false;
        return true;
      }

      enum id_compare_result {
        id_match,
        id_1_contains_2,
        id_2_contains_1,
        id_mismatch
      };

      static id_compare_result compare(id_type &id1, id_type &id2)
      {
        int sz1 = id1.size();
        int sz2 = id2.size();
        int sz = (sz1 < sz2)?sz1:sz2;
        for(int i=0; i<sz; i++) {
          if (id1[i] != id2[i]) {
            if (id1[i] == id_trait::wildcard_token)
              return id_1_contains_2;
            else if (id2[i] == id_trait::wildcard_token)
              return id_2_contains_1;
            else return id_mismatch;
          }
        }
        if (sz1 == sz2)
          return id_match;
        else if(sz1 < sz2 && (sz1+1) == sz2 && id2[sz1] == id_trait::wildcard_token)
          return id_2_contains_1;
        else if(sz2 < sz1 && (sz2+1) == sz1 && id1[sz2] == id_trait::wildcard_token)
          return id_1_contains_2;
        return id_mismatch;
      }

      //compiler doesnt allow references here
      //static bool match(const id_type &id1, const id_type &id2)
      static bool match(id_type id1, id_type id2)
      {
        return compare(id1,id2) != id_mismatch;
      }

      //compiler doesnt allow references here
      //static bool id1contains2(const id_type &id1, const id_type &id2)
      static bool id1contains2(id_type id1, id_type id2)
      {
        id_compare_result res = compare(id1,id2);
        return res == id_match || res == id_1_contains_2;
      }

    };

  }
}

#endif

