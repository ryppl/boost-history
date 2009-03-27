#ifndef BOOST_GUIGL_EXAMPLE_ACCEPTOR_HPP
#define BOOST_GUIGL_EXAMPLE_ACCEPTOR_HPP

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

class IVisitor;

class IAcceptor
  {
  public:
    virtual ~IAcceptor() {}
    virtual void accept(int key, IVisitor const& v) = 0;
  };

template<class T>
class Acceptor :
  public IAcceptor,
  public T
  {
  public:
    explicit Acceptor(T const& obj) : T(obj) {}
    void accept(int key, IVisitor const& v){
      T& obj = *this;
      v.visit(key, obj);
      }

    typedef typename T::result_type result_type;
    result_type const& result() const {return T::result;}
  };

template<class T>
boost::shared_ptr<Acceptor<T> > make_acceptor(T const& g)
  {
  return boost::make_shared<Acceptor<T> >(g);
  }

struct accept_each
  {
  IVisitor const& visitor;
  accept_each(IVisitor const& v):visitor(v){}

  template<class T>
  void operator()(T& obj) const {obj.second->accept(obj.first, visitor);}
  };

#endif
