//
// binder.hpp
//
// Copyright (c) 2005-2009 Yigong Liu (yigongliu at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef BINDER_HPP
#define BINDER_HPP

namespace boost {
  namespace channel {
    /**
     * The name_space filters/translators defined here only manipulte ids/msg-types
     * and affect name_space management
     * message content is UNTOUCHED!
     */

    /**
     * filter_types:
     * 1. provision the permissions which messages/ids can flow in/out
     *    channel thru interfaces
     * 2. ONLY works on sub/pub msgs, not other(application) msgs
     *    affect how sub/pub msgs are exchanged between local channel and
     *    remote channel - how remote and local name_spaces "merge"
     *    not affect the normal message passing process
     * 3. Default filter_types are "transparent" - all Ids allowed in/out:
     *
     */
    template<class id_type>
    class filter_type {
    public:
      virtual bool block_inward(id_type&) {
        return false;
      }
      virtual bool block_outward(id_type&) {
        return false;
      }
      virtual ~filter_type() {
      }
    };

    /**
     * translator_type:
     * 1. translate inward/outward msg ids, help integration of name_spaces
     * 2. ONLY works on application msgs
     *    affect (potentially change) each msgs passed in/out interfaces -
     *    must be highly efficient
     * 3. Default translator_type is non-op
     */
    template<class id_type>
    class translator_type {
    public:
      virtual void translate_inward(id_type&) {
      }
      virtual void translate_outward(id_type &id) {
      }
      virtual ~translator_type() {
      }
    };

    /**
     * binder_type:
     * 1. affect ONLY the interface - the "binding" point between local
     *    channel and remote connections
     * 2. contains both filter and transltor
     * 3. if filters/translators are NULL, they are no-op
     */
    template<class id_type>
    class binder_type {
    public:
      typedef filter_type<id_type> filter_type;
      typedef translator_type<id_type> translator_type;
      filter_type *filter;
      translator_type *translator;
      binder_type(filter_type *f = NULL, translator_type *t = NULL) {
        filter = f;
        translator = t;
      }
      ~binder_type() {
        if (filter != NULL)
          delete filter;
        if (translator != NULL)
          delete translator;
      }
    };

  }
}

#endif

