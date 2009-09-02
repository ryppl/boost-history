// Pinhole signal_info.hpp file
//
// Copyright Jared McIntyre 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PINHOLE_SIGNAL_INFO
#define BOOST_PINHOLE_SIGNAL_INFO

#if defined(BOOST_MSVC)
#pragma warning(push)
#pragma warning( disable: 4561 4793 )
#endif
#include "exceptions.hpp"
#include <boost/type_traits.hpp>
#include <boost/function.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/format.hpp>
#include <boost/any.hpp>
#include <boost/archive/iterators/mb_from_wchar.hpp>
#include <boost/archive/iterators/wchar_from_mb.hpp>
#if defined(BOOST_MSVC)
#pragma warning(pop)
#endif

namespace boost { namespace pinhole { namespace detail
{ 
    struct signal_info_base
    {
    public:
        signal_info_base(const std::type_info &type) : 
            m_type(type)
        {;}
        
        virtual ~signal_info_base(){;}
        
        std::string           m_name;
        boost::any            m_metadata;
        const std::type_info &m_type;
        
    private:
        signal_info_base(const property_info_base&):m_type(typeid(void)){;} // this would be bad
    };

    template<typename SignalType>
    struct signal_info : signal_info_base
    {
        
    public:        
        // Signals must be references
        BOOST_STATIC_ASSERT(true == boost::is_reference<SignalType>::value);
        
        SignalType m_signal;
        
        signal_info(SignalType signal) : signal_info_base(typeid(SignalType)), m_signal(signal) {;}
        
    private:
        signal_info() : signal_info_base(typeid(void)){;} // this would be bad
        signal_info(const signal_info<SignalType>&) : signal_info_base(typeid(void)){;} // this would be bad
    };
}}}

#endif // include guard