//
// stream_state.hpp - maintain user-defined state per stream.
//
// Copyright (c) 2007 Jeff Faust
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// The single entry point, get_stream_state, wraps the arcane ios
// interfaces xalloc, pword, and register_callback.

#ifndef EXPLOER_STREAM_STATE_INCLUDED
#define EXPLOER_STREAM_STATE_INCLUDED

#include <ios> 
#include <memory>

namespace boost { namespace explore
{
    // forward declaration for deletion callback
    template<typename T>
    T* get_stream_state(std::ios_base& stream, bool create = true);

    namespace detail
    {
        // returns an index into the ios_base container of user-defined data.  This will
        // create exactly one index per type T.
        template<typename T>
        int get_stream_state_index()
        {
            static int index = std::ios_base::xalloc();
            return index;
        }

        // callback for extra state events
        template<typename T>
        void extra_state_callback(std::ios_base::event e, std::ios_base& stream, int arg)
        {
            if( arg == get_stream_state_index<T>() )
            {
                switch( e )
                {
                case std::ios_base::erase_event:
                    delete get_stream_state<T>(stream);
                    stream.pword(arg) = 0;
                    break;
                case std::ios_base::copyfmt_event:
                {
                    // format has been copied.  Make an actual copy instead of using the same
                    // pointer.  Prevents deleting memory twice.
                    T* state = get_stream_state<T>(stream);

                    // first set to 0 in case allocation fails.  Although copyfmt will have failed,
                    // we at least won't crash when we won't delete the same memory twice
                    stream.pword(arg) = 0;
                    stream.pword(arg) = new T(*state);
                    break;
                }
                case std::ios_base::imbue_event:
                    break;
                }
            }
        }
    }

    // returns state information stored in the stream for the given type.  Will allocate
    // state data if needed and create == true.  If create == false and the data does not
    // already exist, it will return 0.
    // T: the user-defined state class.  Must be constructible, accepting a stream pointer.
    template<typename T>
    T* get_stream_state(std::ios_base& stream, bool create)
    {
        // grab reserved index
        int index = detail::get_stream_state_index<T>();

        // grab state data at that index, casting from void*
        T*& state = reinterpret_cast<T*&>(stream.pword(index));
        if( !state && create )
        {
            // both creating a new T and registering the callback allocate memory.  Use
            // auto_ptr to satisfy the strong exception guarantee.
            std::auto_ptr<T> pt(new T(&stream));
            stream.register_callback(detail::extra_state_callback<T>, index);
            state = pt.release();
        }
        return state;
    }
}}

#endif
