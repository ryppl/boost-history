#ifndef BOOST_GUIGL_EXAMPLE_VISITOR_HPP
#define BOOST_GUIGL_EXAMPLE_VISITOR_HPP

#include "functor.hpp"

class IVisitor
  {
  public:
    virtual ~IVisitor(){}

    virtual void visit(source_point& g) const {}
    virtual void visit(line_from_two_points& g) const {};
    virtual void visit(plane_from_three_points& g) const {};
  };

template<class StaticVisitor>
class Visitor : public IVisitor
  {
  private:
    StaticVisitor m_static_visitor;

  public:
    Visitor(StaticVisitor const& static_visitor = StaticVisitor())
      : m_static_visitor(static_visitor) {}
    void visit(source_point& g) const {m_static_visitor(g);}
    void visit(line_from_two_points& g) const {m_static_visitor(g);};
    void visit(plane_from_three_points& g) const {m_static_visitor(g);};
  };

#endif
