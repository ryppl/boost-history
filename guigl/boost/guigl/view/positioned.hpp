/*=================================---------------------------------------------
    Copyright 2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/

#ifndef BOOST__GUIGL__VIEW__POSITIONED_HPP
#define BOOST__GUIGL__VIEW__POSITIONED_HPP

#include <boost/guigl/view/base.hpp>
#include <boost/mpl/bool.hpp>

namespace boost { namespace guigl { namespace view {

template<typename View>
struct is_positioned
    : public is_positioned<typename View::base_type>
{};

template<>
struct is_positioned<base> : public mpl::false_
{};

template<typename BaseView=base>
class positioned : public BaseView
{
    typedef BaseView base_type;
public:
    template<typename ArgumentPack>
    positioned(const ArgumentPack &args)
        : base_type(args)
        , m_position(args[_position])
        , m_parent(0)
    {}
    
    const position_type &position() const
    {   return m_position; }

    const base *root() const
    {
        const base *root = this;
        const positioned<> *p_view;
        while((p_view = dynamic_cast<const positioned<> *>(root)))
            root = p_view->m_parent;
        return root;
    }

protected:
    BOOST_GUIGL_DRAW

    friend class access;
    void parent(base &parent)
    {   m_parent = &parent; }
private:

    position_type m_position;
    base *m_parent;
};

template<typename BaseView>
struct is_positioned<positioned<BaseView> > : public mpl::true_
{};


}}}

#endif // BOOST__GUIGL__VIEW__POSITIONED_HPP