// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_DATAFLOW_BLUEPRINT_KEYED_PORT_T_HPP
#define BOOST_DATAFLOW_BLUEPRINT_KEYED_PORT_T_HPP

#include <boost/dataflow/blueprint/vector_port_t.hpp>
#include <boost/dataflow/support/keyed_port.hpp>

#include <boost/array.hpp>
#include <boost/mpl/for_each.hpp>
#include <boost/mpl/identity.hpp>

namespace boost { namespace dataflow { namespace blueprint {

namespace detail {
    
    template<typename T>
    struct wrap_identity
    {
        typedef mpl::identity<T> type;
    };
    
    template<typename Array>
    struct fill_type_info
    {
        fill_type_info(Array &array) : m_array(array), it() {}
        
        template<typename T>
        void operator()(T)
        {
            m_array[it++]=&typeid(typename T::type);
        }
        
        Array &m_array;
        int it;
    };
}

template<typename PortOrRef, typename Tag>
class port_t<
    PortOrRef,
    Tag,
    typename enable_if<
        is_keyed_port<
            typename remove_reference<PortOrRef>::type,
            Tag>
    >::type >
    : public detail::vector_port_impl<keyed_port, PortOrRef, Tag>
{
    typedef detail::vector_port_impl<keyed_port, PortOrRef, Tag> base_type;
    typedef typename base_type::port_traits_type port_traits_type;

    typedef array<
        const std::type_info *,
        mpl::size<typename port_traits_type::port_keys>::type::value
    >  array_type;
    
public:
    port_t(typename call_traits<PortOrRef>::param_type p)
        : detail::vector_port_impl<keyed_port, PortOrRef, Tag>(p)
    {
        mpl::for_each<
            typename port_traits_type::port_keys,
            detail::wrap_identity<mpl::_>
        > (detail::fill_type_info<array_type>(m_type_info));
    }
    
    virtual port *clone() const
    {
        return new port_t(*this);
    }
    
    bool has_key(const std::type_info &info)
    {
        for(typename array_type::iterator it=m_type_info.begin(); it!= m_type_info.end(); it++)
            if (**it == info)
                return true;
        return false;
    }
    port &get_keyed_port(const std::type_info &info)
    {
        for(typename array_type::iterator it=m_type_info.begin(); it!= m_type_info.end(); it++)
            if (**it == info)
                return port_t::m_ports[it - m_type_info.begin()];
        throw std::exception();
    }
private:
    array_type m_type_info;
};

} } } // namespace boost::dataflow::blueprint

#endif // BOOST_DATAFLOW_BLUEPRINT_KEYED_PORT_T_HPP
