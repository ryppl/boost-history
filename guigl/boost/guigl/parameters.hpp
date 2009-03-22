/*=================================---------------------------------------------
    Copyright 2008,2009 Stjepan Rajko, Andrey Torba
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/

#ifndef BOOST__GUIGL__PARAMETER_MAP_HPP
#define BOOST__GUIGL__PARAMETER_MAP_HPP

#include <boost/guigl/event.hpp>
#include <boost/guigl/types.hpp>
#include <boost/parameter/typed_name.hpp>
#include <boost/guigl/color.hpp>

namespace boost { namespace guigl {

namespace keywords {
    BOOST_PARAMETER_TYPED_NAME_WDEFAULT(label,const std::string,"")
    BOOST_PARAMETER_TYPED_NAME_WDEFAULT(size,const size_type,(size_type(200,200)))
    BOOST_PARAMETER_TYPED_NAME_WDEFAULT(position,const position_type,(size_type(0,0)))
    BOOST_PARAMETER_TYPED_NAME_WDEFAULT(background,const color_type,(black()))
    BOOST_PARAMETER_TYPED_NAME_WDEFAULT(color,const color_type,(white()))
    BOOST_PARAMETER_TYPED_NAME_WDEFAULT(active_color,const color_type,(red()))
    BOOST_PARAMETER_TYPED_NAME_WDEFAULT(depth,const bool,false)
    BOOST_PARAMETER_TYPED_NAME_WDEFAULT(min,const double,0.0)
    BOOST_PARAMETER_TYPED_NAME_WDEFAULT(max,const double,1.0)
    BOOST_PARAMETER_TYPED_NAME_WDEFAULT(step,const double,0.0)
    BOOST_PARAMETER_TYPED_NAME_WDEFAULT(period,const double,0.0)

    BOOST_PARAMETER_TYPED_NAME_WDEFAULT(clickable_button,button::enum_type,button::left)
    BOOST_PARAMETER_TYPED_NAME_WDEFAULT(draggable_button,button::enum_type,button::left)

    BOOST_PARAMETER_UNTYPED_NAME(value)
    BOOST_PARAMETER_UNTYPED_NAME(children)
    BOOST_PARAMETER_UNTYPED_NAME(draw_prologue)
    BOOST_PARAMETER_UNTYPED_NAME(draw_epilogue)
    BOOST_PARAMETER_UNTYPED_NAME(on_event)
    
    typedef boost::parameter::aux::empty_typed_arg_list default_parameters;
}

using namespace keywords;

template<
    typename T0=boost::parameter::void_, typename T1=boost::parameter::void_,
    typename T2=boost::parameter::void_, typename T3=boost::parameter::void_,
    typename T4=boost::parameter::void_, typename T5=boost::parameter::void_,
    typename T6=boost::parameter::void_, typename T7=boost::parameter::void_>
struct argument_pack
{
    typedef
        boost::parameter::aux::typed_arg_list<
            boost::parameter::aux::typed_tagged_argument<T0, typename T0::value_type>,
            typename argument_pack<T1, T2, T3, T4, T5, T6, T7>::type
        > type;
};

template<>
struct argument_pack<
    boost::parameter::void_, boost::parameter::void_,
    boost::parameter::void_, boost::parameter::void_,
    boost::parameter::void_, boost::parameter::void_,
    boost::parameter::void_, boost::parameter::void_>
{
    typedef boost::parameter::aux::empty_typed_arg_list type;
};

}}

#endif // BOOST__GUIGL__PARAMETER_MAP_HPP
