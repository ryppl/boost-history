//
// state_saver.hpp - save/restore container state with a scoped object
//
// Copyright (c) 2009 Jeff Faust
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// state_saver works like the boost I/O boost stream state saver, but on
// container state values.
//
// See http://www.boost.org/libs/explore for library home page.

#ifndef BOOST_EXPLORE_STATE_SAVER_H
#define BOOST_EXPLORE_STATE_SAVER_H

#include <boost/explore/container_stream_state.hpp>

namespace boost
{
    namespace explore
    {
        template<typename Elem>
        class state_saver
        {
        public:
            explicit state_saver(std::ios_base& ostr)
             : m_state(get_stream_state<container_stream_state<Elem> >(ostr)),
               m_save_state(*m_state),
               m_common_state(
                    get_stream_state<container_common_stream_state>(ostr)),
               m_save_common_state(*m_common_state), m_restored(false)
            {
            }

            ~state_saver()
            {
                restore();
            }

            void restore()
            {
                if( !m_restored ) // don't double-swap
                {
                    m_state->swap(m_save_state);
                    m_common_state->swap(m_save_common_state);
                    m_restored = true;
                }
            }

        private:
            container_stream_state<Elem>* m_state;
            container_stream_state<Elem> m_save_state;

            container_common_stream_state* m_common_state;
            container_common_stream_state m_save_common_state;

            bool m_restored;
        };
    }
}

#endif
