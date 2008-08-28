/*=================================---------------------------------------------
    Copyright 2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/

#ifndef BOOST__GUIGL__LAYOUT__GRID_HPP
#define BOOST__GUIGL__LAYOUT__GRID_HPP


#include <boost/guigl/view/base.hpp>
#include <boost/guigl/types.hpp>
#include <memory>


namespace boost { namespace guigl {

namespace major_direction
{
    enum enum_type
    {
        horizontal=0,vertical=1
    };
}

namespace keywords {

    BOOST_PARAMETER_TYPED_NAME(grid_size,const size_type)
    BOOST_PARAMETER_TYPED_NAME_WDEFAULT(horizontal,const unsigned,1)
    BOOST_PARAMETER_TYPED_NAME_WDEFAULT(vertical,const unsigned,1)
    BOOST_PARAMETER_TYPED_NAME_WDEFAULT(direction,const major_direction::enum_type,major_direction::horizontal)
    
    typedef boost::parameter::aux::empty_typed_arg_list default_parameters;
}


namespace layout {

class grid
{
public:
    template<typename ArgumentPack>
    grid(const ArgumentPack &args)
        : m_size(args[_grid_size])
        , m_element_size(m_size.x / args[_horizontal], m_size.y/args[_vertical])
        , m_direction(args[_direction])
    {
        m_dimensions[0] = args[_horizontal];
        m_dimensions[1] = args[_vertical];
        m_next[0] = m_next[1] = 0;
    }
    
    template<typename T, typename ArgumentPack>
    T *create(const ArgumentPack &args)
    {
        T *element(new T((
            args,
            _size = m_element_size,
            _position = position_type(m_element_size.x * m_next[0], m_element_size.y * m_next[1])
        )) );
        
        if(++m_next[m_direction] >= m_dimensions[m_direction])
        {
            m_next[m_direction] = 0;
            m_next[!m_direction]++;
        }
        return element;
    }
private:
    size_type m_size;
    size_type m_element_size;
    unsigned m_dimensions[2];
    unsigned m_next[2];
    major_direction::enum_type m_direction;
};

}}}

#endif // BOOST__GUIGL__VIEW__POSITIONED_HPP