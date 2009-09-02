// Pinhole action_info.hpp file
//
// Copyright Jared McIntyre 2007.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PINHOLE_ACTION_INFO
#define BOOST_PINHOLE_ACTION_INFO

#if defined(BOOST_MSVC)
    #pragma warning(push)
    #pragma warning( disable: 4561 4793 )
#endif
#include <boost/type_traits.hpp>
#include <boost/function.hpp>
#include <boost/lexical_cast.hpp>
#if defined(BOOST_MSVC)
    #pragma warning(pop)
#endif

namespace boost { namespace pinhole { namespace detail
{
    struct action_info
    {
    private:
        template<typename type> struct Internal;

    public:
        typedef boost::function<void ()> action_type;
        
        virtual ~action_info(){;}

        action_type m_action;
        std::string m_name;
        boost::any  m_metadata;

        /**
         * Calls the appropriate action function for this parameter.
         *
         * @throw boost::bad_function_call There isn't a set function associated with this property.
         */
        virtual void trigger()
        {
            m_action();
        }
    };
}}}

#endif // include guard