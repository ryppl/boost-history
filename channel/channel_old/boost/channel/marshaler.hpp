//
// marshaler.hpp
//
// Copyright (c) 2005-2009 Yigong Liu (yigongliu at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef MARSHALER_HPP
#define MARSHALER_HPP

#include <vector>
#include <map>
#include <boost/shared_ptr.hpp>
#include <boost/channel/message.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/binary_object.hpp>
#include <boost/serialization/vector.hpp>

/**
 * marshaling & demarshaling : msg converting : typed info <-> untyped stream
 * here we use Boost.Serialization text_archive for conversion.
 * other choices: binary archive or xml archives
 *
 * 2 steps to enable marshaling for a message type:
 * 1> define a free serialize function for it (non-intrusive)
 * 2> register the type with marshaler_registry:
 *        marshaler_registry.register<msg_type>(id);
 *
 * for msg types using PODS: struct without virtual methods, dont
 * need define any serialization methods or register marshalers, the
 * default marshaler<void_type> does byte-copy and will be enough
 */

namespace boost {
  namespace channel {

    ///interface of all message marshalers
    class msg_marshaler {
    public:
      virtual void marshal(boost::shared_ptr<void> &msg, std::string &buf) = 0;
      virtual void demarshal(std::string &buf, boost::shared_ptr<void> &msg) = 0;
      virtual ~msg_marshaler() {
      }
    };

    ///interface of all id marshalers
    template<typename id_type>
    class id_marshaler {
    public:
      virtual void marshal(const id_type &id, std::string &buf) = 0;
      virtual void demarshal(std::string &buf, id_type &id) = 0;
      virtual ~id_marshaler() {
      }
    };

    ///grouping of archives
    enum marshaling_archive_type {
      text_archive, binary_archive, xml_archive
    };

    template<int> class marshaling_archives;

    template<>
    struct marshaling_archives<text_archive> {
      typedef boost::archive::text_oarchive oarchive;
      typedef boost::archive::text_iarchive iarchive;
    };

    template<>
    struct marshaling_archives<binary_archive> {
      typedef boost::archive::binary_oarchive oarchive;
      typedef boost::archive::binary_iarchive iarchive;
    };

    template<>
    struct marshaling_archives<xml_archive> {
      typedef boost::archive::xml_oarchive oarchive;
      typedef boost::archive::xml_iarchive iarchive;
    };

    /**
     * --------- id marshaler types --------------
     */
    template<typename id_type, int archive_type>
    class id_marshaler_impl: public id_marshaler<id_type> {
      void marshal(const id_type &id, std::string &buf) {
        std::ostringstream archive_stream;
        typename marshaling_archives<archive_type>::oarchive archive(archive_stream);
        archive << id;
        buf = archive_stream.str();
      }
      void demarshal(std::string &buf, id_type &id) {
        std::istringstream archive_stream(buf);
        typename marshaling_archives<archive_type>::iarchive archive(archive_stream);
        archive >> id;
      }
    };

    /**
     * --------- message marshaler types --------------
     */
    //struct void_type {};

    ///---- text_marshaler: marshal typed message using text_archive ----
    template<typename msg_type, int archive_type>
    class msg_marshaler_impl: public msg_marshaler {
    public:
      void marshal(boost::shared_ptr<void> &msg, std::string &buf) {
        /// must be const pointer, otherwise marshaling will fail
        msg_type * const data = (msg_type *) msg.get();
        std::ostringstream archive_stream;
        typename marshaling_archives<archive_type>::oarchive archive(archive_stream);
        archive << data;
        buf = archive_stream.str();
      }
      void demarshal(std::string &buf, boost::shared_ptr<void> &msg) {
        msg_type * data; ///to support marshaling, msg_type need support default constructor
        std::istringstream archive_stream(buf);
        typename marshaling_archives<archive_type>::iarchive archive(archive_stream);
        archive >> data;
        msg.reset(data);
      }
    };

    /*
   /// binary copy text_marshaler, used as default
   template <int archive_type>
   struct msg_marshaler_impl<void_type, archive_type> : public msg_marshaler {
   void marshal(boost::shared_ptr<void> &msg, int sz, std::string &buf)
   {
   std::ostringstream archive_stream;
   typename marshaling_archives<archive_type>::oarchive archive(archive_stream);
   archive << boost::serialization::make_binary_object(msg.get(), sz);
   buf = archive_stream.str();
   }
   void demarshal(std::string &buf, boost::shared_ptr<void> &msg, int &sz)
   {
   sz = buf.size();
   char *data = new char[sz]; //for binary obj, we have to alloc space
   std::istringstream archive_stream(buf);
   typename marshaling_archives<archive_type>::iarchive archive(archive_stream);
   archive >> boost::serialization::make_binary_object(data, sz);
   msg.reset(data);
   }
   };
    */

    /**
     * -------------- xml_marshaler -----------
     */

    /**
     * marshaler registration map:
     * for simple application, all remote connections can share a marshaler_registry
     * for applications involving diff marshaling strategies for diff remote
     * connections, several marshaler_registries can be used; each stream object
     * will use its own marshaler_registry
     */

    template<typename id_type, typename id_trait, typename synch_policy, int archive_type = text_archive>
    class marshaler_registry {
    private:
      //marshaler registration
      std::map<id_type, boost::shared_ptr<msg_marshaler> > mar_tbl_;
      //msg_marshaler_impl<void_type, archive_type> bin_mar_;
      id_marshaler_impl<id_type, archive_type> id_mar_;
      msg_marshaler *default_mar_;
      typename synch_policy::mutex mar_tbl_lock_;
    public:
      typedef id_marshaler<id_type> id_marshaler;
      marshaler_registry() :
        default_mar_(NULL) {
        //register marshaler for system internal msgs
        std::vector<id_type> ids;
        ids.push_back(id_trait::channel_conn_msg);
        ids.push_back(id_trait::channel_disconn_msg);
        ids.push_back(id_trait::connection_ready_msg);
        register_msg_marshaler<channel_info_msg_t> (ids);

        ids.clear();
        ids.push_back(id_trait::init_subscription_info_msg);
        ids.push_back(id_trait::subscription_info_msg);
        ids.push_back(id_trait::unsubscription_info_msg);
        ids.push_back(id_trait::publication_info_msg);
        ids.push_back(id_trait::unpublication_info_msg);
        register_msg_marshaler<pubsub_info_msg_t<id_type> > (ids);
      }

      ~marshaler_registry() {
        if (default_mar_ != NULL)
          delete default_mar_;
      }

      template<typename msg_type>
      void register_default_msg_marshaler(void) {
        typename synch_policy::scoped_lock lock(mar_tbl_lock_);
        if (default_mar_ != NULL)
          delete default_mar_;
        default_mar_ = new msg_marshaler_impl<msg_type, archive_type> ();
      }

      template<typename msg_type>
      void register_msg_marshaler(id_type id) {
        typename synch_policy::scoped_lock lock(mar_tbl_lock_);
        boost::shared_ptr<msg_marshaler> mar(new msg_marshaler_impl<msg_type, archive_type> ());
        mar_tbl_[id] = mar;
      }

      template<typename msg_type>
      void register_msg_marshaler(std::vector<id_type> &ids) {
        typename synch_policy::scoped_lock lock(mar_tbl_lock_);
        boost::shared_ptr<msg_marshaler> mar(new msg_marshaler_impl<msg_type, archive_type> ());
        for (typename std::vector<id_type>::iterator iter = ids.begin(); iter != ids.end(); iter++) {
          mar_tbl_[*iter] = mar;
        }
      }

      msg_marshaler * get_msg_marshaler(id_type id) {
        typename synch_policy::scoped_lock lock(mar_tbl_lock_);
        if (mar_tbl_.find(id) != mar_tbl_.end())
          return mar_tbl_[id].get();
        else
          //if(default_mar_ != NULL)
          return default_mar_;
        /*
          else
          return &bin_mar_; //default byte-copy
        */
      }

      //return default marshaler
      msg_marshaler * get_msg_marshaler(void) {
        //if(default_mar_ != NULL)
        return default_mar_;
        //return &bin_mar_;
      }

      id_marshaler * get_id_marshaler(void) {
        return &id_mar_;
      }
    };

  }
}

#endif
