#ifndef BOOST_GUIGL_EXAMPLE_VISITOR_HPP
#define BOOST_GUIGL_EXAMPLE_VISITOR_HPP

#include "functor.hpp"

class IVisitor
  {
  public:
    virtual ~IVisitor(){}

    //Vector
    virtual void visit(functor::perpendicular_to& g) const {}
    virtual void visit(functor::normal_of& g) const {}
    virtual void visit(functor::direction_of& g) const {}
    virtual void visit(functor::translate_line& g) const {}

    //Point
    virtual void visit(functor::source_point& g) const {}

    //Line
    virtual void visit(functor::line_from_two_points& g) const {}

    //Plane
    virtual void visit(functor::plane_from_three_points& g) const {}
  };

template<class StaticVisitor>
class Visitor : public IVisitor
  {
  private:
    StaticVisitor m_static_visitor;

  public:
    Visitor(StaticVisitor const& static_visitor = StaticVisitor())
      : m_static_visitor(static_visitor) {}

    //Vector
    void visit(functor::perpendicular_to& g) const {m_static_visitor(g);}
    void visit(functor::normal_of& g) const {m_static_visitor(g);}
    void visit(functor::direction_of& g) const {m_static_visitor(g);}
    void visit(functor::translate_line& g) const {m_static_visitor(g);}

    //Point
    void visit(functor::source_point& g) const {m_static_visitor(g);}

    //Line
    void visit(functor::line_from_two_points& g) const {m_static_visitor(g);}

    //Plane
    void visit(functor::plane_from_three_points& g) const {m_static_visitor(g);}
  };

#endif
