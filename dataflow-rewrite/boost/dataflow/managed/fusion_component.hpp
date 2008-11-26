// Copyright 2008 Stjepan Rajko.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_DATAFLOW_MANAGED_FUSION_COMPONENT_HPP
#define BOOST_DATAFLOW_MANAGED_FUSION_COMPONENT_HPP

#include <boost/dataflow/detail/make_ref.hpp>
#include <boost/dataflow/generic/fusion_static_vector.hpp>
#include <boost/dataflow/managed/port.hpp>
#include <boost/dataflow/managed/component.hpp>
//#include <boost/dataflow/support/component_operation.hpp>
#include <boost/dataflow/utility/forced_sequence.hpp>
//#include <boost/fusion/container/lazy_sequence.hpp>
#include <boost/fusion/include/as_vector.hpp>
#include <boost/fusion/include/size.hpp>
#include <boost/fusion/include/transform.hpp>
#include <boost/mpl/map.hpp>
#include <boost/mpl/joint_view.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/range_c.hpp>
#include <boost/mpl/transform.hpp>


namespace boost { namespace dataflow { namespace managed {

namespace detail
{
    struct add_reference;
    
    template<typename T, typename PortCategory>
    struct make_managed_port
    {
        typedef managed::port<T, PortCategory> type;
    };
    
    template<typename ConsumerSignatures, typename ProducerSignatures>
    struct make_fusion_ports
    {
        typedef typename fusion::result_of::as_vector<
            mpl::joint_view<
                typename mpl::transform<
                    ConsumerSignatures,
                    make_managed_port<mpl::_1, ports::consumer>
                >::type,
                typename mpl::transform<
                    ProducerSignatures,
                    make_managed_port<mpl::_1, ports::producer>
                >::type
            >
        >::type type;
    };

}

template<typename InTypes, typename OutTypes>
struct component_traits
    : public dataflow::fusion_static_vector_traits<
    boost::fusion::transform_view<
            typename detail::make_fusion_ports<InTypes, OutTypes>::type,
            boost::dataflow::detail::make_ref
        >,
        managed::framework>
{
    typedef typename detail::make_fusion_ports<InTypes, OutTypes>::type fusion_ports;

    template<typename Component>
    static typename component_traits::fusion_ports &get_ports(Component &component)
    {
        return component.ports();
    };
};

namespace detail {

    struct component_f
    {
        typedef component &result_type;
        
        component_f(component &c)
            : m_component(c) {}
        
        template<typename Index>
        result_type operator()(Index) const
        {   return m_component; }
        
        mutable result_type m_component;
    };

}

template<typename InTypes, typename OutTypes=InTypes>
class fusion_component : public component
{
public:
    typedef typename dataflow::utility::forced_sequence<InTypes>::type in_types_sequence;
    typedef typename dataflow::utility::forced_sequence<OutTypes>::type out_types_sequence;
    
    typedef component_traits<in_types_sequence, out_types_sequence> dataflow_traits;
    typedef typename detail::make_fusion_ports<in_types_sequence, out_types_sequence>::type ports_type;
    
    fusion_component(network &n)
        : component(n)
        , m_ports(fusion::transform(mpl::range_c<int,0,fusion::result_of::size<ports_type>::type::value>(), detail::component_f(*this)))
    {}
    template<int Index>
    typename fusion::result_of::at_c<ports_type, Index>::type port()
    {   return fusion::at_c<Index>(m_ports); }
    ports_type &ports()
    {   return m_ports; }
protected:
    ports_type m_ports;
};

}

namespace extension {

/*    template<typename InTypesSequence, typename OutTypesSequence>
    struct component_operation_impl<managed::component_traits<InTypesSequence,OutTypesSequence>, operations::invoke >
    {
        typedef void result_type;
        
        template<typename Component>
        void operator()(Component &component)
        {
            component.invoke();
        }
    };*/

}

} }

#endif // BOOST_DATAFLOW_MANAGED_FUSION_COMPONENT_HPP
