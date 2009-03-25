/*=================================---------------------------------------------
    Copyright 2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/

#ifndef BOOST__GUIGL__VIEW__BASE_HPP
#define BOOST__GUIGL__VIEW__BASE_HPP

#include <boost/guigl/access.hpp>
#include <boost/guigl/event.hpp>
#include <boost/guigl/export_symbols.hpp>
#include <boost/guigl/parameters.hpp>

namespace boost { namespace guigl {

class window;

struct LT {}; // left-top point
struct LC {}; // left-center point
struct LB {}; // left-bottom point
struct CT {}; // center-top point
struct CC {}; // center-center point
struct CB {}; // center-bottom point
struct RT {}; // right-top point
struct RC {}; // right-center point
struct RB {}; // right-bottom point

struct VL {}; // vertical left segment
struct VC {}; // vertical center segment
struct VR {}; // vertical right segment
struct HT {}; // horizontal left segment
struct HC {}; // horizontal center segment
struct HB {}; // horizontal right segment
struct D1 {}; // LT-RB diagonal
struct D2 {}; // LB-RT diagonal

namespace view {

class base
{

public:
    template<typename ArgumentPack>
    base(const ArgumentPack &args)
        : m_size(args[_size])
    {}
    virtual ~base()
    {}
    
    const size_type &size() const
    {   return m_size; }

    void set_size(const size_type &size)
    {   m_size = size; }

    template<class PositionTag>
    position_type point() const;

#define BOOST_GUIGL_SPECIAL_POINT_IMPL(point_tag_, val1_, val2_)  \
  template<>                                                      \
    inline position_type point<point_tag_>() const                \
      { return position_type(val1_, val2_); }
  
    BOOST_GUIGL_SPECIAL_POINT_IMPL(LT, 0., 0.);
    BOOST_GUIGL_SPECIAL_POINT_IMPL(LC, 0., size().y/2);
    BOOST_GUIGL_SPECIAL_POINT_IMPL(LB, 0., size().y);
    BOOST_GUIGL_SPECIAL_POINT_IMPL(CT, size().x/2, 0.);
    BOOST_GUIGL_SPECIAL_POINT_IMPL(CC, size().x/2, size().y/2);
    BOOST_GUIGL_SPECIAL_POINT_IMPL(CB, size().x/2, size().y);
    BOOST_GUIGL_SPECIAL_POINT_IMPL(RT, size().x, 0.);
    BOOST_GUIGL_SPECIAL_POINT_IMPL(RC, size().x, size().y/2);
    BOOST_GUIGL_SPECIAL_POINT_IMPL(RB, size().x, size().y);

    template<class SegmentTag>
    inline segment_type segment() const;

#define BOOST_GUIGL_SPECIAL_SEGMENT_IMPL(segment_tag_, point_tag1_, point_tag2_)    \
  template<>                                                                        \
  inline segment_type segment<segment_tag_>() const                                 \
      {                                                                             \
      return segment_type(point<point_tag1_>(), point<point_tag2_>());              \
      }

    BOOST_GUIGL_SPECIAL_SEGMENT_IMPL(VL, LT, LB);
    BOOST_GUIGL_SPECIAL_SEGMENT_IMPL(VC, CT, CB);
    BOOST_GUIGL_SPECIAL_SEGMENT_IMPL(VR, RT, RB);
    BOOST_GUIGL_SPECIAL_SEGMENT_IMPL(HT, LT, RT);
    BOOST_GUIGL_SPECIAL_SEGMENT_IMPL(HC, LC, RC);
    BOOST_GUIGL_SPECIAL_SEGMENT_IMPL(HB, LB, RB);
    BOOST_GUIGL_SPECIAL_SEGMENT_IMPL(D1, LT, RB);
    BOOST_GUIGL_SPECIAL_SEGMENT_IMPL(D2, RT, LB);

protected:
    virtual void draw() = 0;
    void draw_prologue()
      {
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      }

    void draw_epilogue()
      {
      glDisable(GL_BLEND);
      }
    
    virtual bool on_event(const event_type &e)
    {
        return false;
    };

    size_type m_size;
    
    friend class boost::guigl::access;
};

}}}


#define BOOST_GUIGL_WIDGET_DRAW             \
    friend class ::boost::guigl::access;    \
    void BOOST_EXPORT_SYMBOLS draw();       \
    void draw_prologue();                   \
    void draw_epilogue();                   \

#define BOOST_GUIGL_DRAW_IMPL(view_type)    \
template<typename BaseView>                 \
void view_type<BaseView>::draw()            \
{                                           \
    draw_prologue();                        \
    draw_epilogue();                        \
}                                           \

#define BOOST_GUIGL_WIDGET_DRAW_IMPL(widget)\
void widget::draw()                         \
{                                           \
    draw_prologue();                        \
    draw_epilogue();                        \
}                                           \


#endif // BOOST__GUIGL__VIEW_HPP
