// Boost.Signals library
//
// Copyright (C) 2001 Doug Gregor (gregod@cs.rpi.edu)
//
// Permission to copy, use, sell and distribute this software is granted
// provided this copyright notice appears in all copies.
// Permission to modify the code and to distribute modified code is granted
// provided this copyright notice appears in all copies, and a notice
// that the code was modified is included with the copyright notice.
//
// This software is provided "as is" without express or implied warranty,
// and with no claim as to its suitability for any purpose.
 
// For more information, see http://www.boost.org

#ifndef BOOST_SIGNALS_SLOT_HEADER
#define BOOST_SIGNALS_SLOT_HEADER

#include <boost/signals/detail/signals_common.hpp>
#include <boost/signals/connection.hpp>
#include <boost/signals/trackable.hpp>
#include <boost/visit_each.hpp>
#include <cassert>

namespace boost {
  namespace signals {
    namespace detail {
      class slot_base {
        // We would have to enumerate all of the signalN classes here as 
        // friends to make this private (as it otherwise should be). We can't
        // name all of them because we don't know how many there are.
      public:
        // Get the set of bound objects
        std::vector<const trackable*>& get_bound_objects() const 
        { return bound_objects; }
        
        // Determine if this slot is still "active", i.e., all of the bound
        // objects still exist
        bool is_active() const { return watch_bound_objects.connected(); }

      protected:
        // Create a connection for this slot
        void create_connection();

        // Get the slot so that it can be copied
        template<typename F> 
        reference_wrapper<const F> 
        get_invocable_slot(const F& f, signals::detail::signal_tag)
          { return reference_wrapper<const F>(f); }

        template<typename F> 
        const F& get_invocable_slot(const F& f, signals::detail::reference_tag)
          { return f; }

        template<typename F>
        const F& get_invocable_slot(const F& f, signals::detail::value_tag)
          { return f; }

        // Get the slot so that it can be inspected for trackable objects
        template<typename F> 
        const F& get_inspectable_slot(const F& f,
                                      signals::detail::signal_tag)
          { return f; }

        template<typename F> 
        const F& get_inspectable_slot(const F& f, 
                                      signals::detail::reference_tag)
          { return f.get(); }

        template<typename F>
        const F& get_inspectable_slot(const F& f, signals::detail::value_tag)
          { return f; }

        // Determines the type of the slot - is it a signal, a reference to a
        // slot or just a normal slot.
        template<typename F>
        typename signals::detail::get_slot_tag<F>::type
        tag_type(const F&)
        {
          typename signals::detail::get_slot_tag<F>::type tag;
          return tag;
        }

        mutable std::vector<const trackable*> bound_objects;
        connection watch_bound_objects;

      private:
        static void bound_object_destructed(void*, void*) {}
      };
    } // end namespace detail
  } // end namespace signals

  template<typename SlotFunction>
  class slot : public signals::detail::slot_base {
  public:
    template<typename F> 
    slot(const F& f) : slot_function(get_invocable_slot(f, tag_type(f)))
    {
      // Visit each of the bound objects and store them for later use
      // An exception thrown here will allow the basic_connection to be
      // destroyed when this goes out of scope, and no other connections
      // have been made.
      signals::detail::bound_objects_visitor do_bind(bound_objects);
      visit_each(do_bind, get_inspectable_slot(f, tag_type(f)));

      create_connection();
    }

#ifdef __BORLANDC__
    template<typename F>
    slot(F* f) : slot_function(f)
    {
      create_connection();
    }
#endif // __BORLANDC__

    // We would have to enumerate all of the signalN classes here as friends
    // to make this private (as it otherwise should be). We can't name all of
    // them because we don't know how many there are.
  public: 
    // Get the slot function to call the actual slot
    const SlotFunction& get_slot_function() const { return slot_function; }

  private:
    slot(); // no default constructor
    slot& operator=(const slot&); // no assignment operator

    SlotFunction slot_function;
  };
} // end namespace boost

#endif // BOOST_SIGNALS_SLOT_HEADER
