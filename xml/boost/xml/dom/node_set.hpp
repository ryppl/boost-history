#ifndef boost_xml_dom_node_set_hpp_
#define boost_xml_dom_node_set_hpp_

#include <libxml/xpathInternals.h>
#include <boost/xml/dom/node.hpp>

namespace boost
{
namespace xml
{
namespace dom
{

template <typename S>
class node_set : public detail::wrapper<xmlNodeSet*>
{
  friend class xpath<S>;
public:
  class iterator;

  ~node_set() { xmlXPathFreeNodeSet(this->impl());}

  size_t size() const { return this->impl() ? this->impl()->nodeNr : 0;}
  node_ptr<node<S> > operator[] (size_t i) 
  {
    return detail::ptr_factory<node<S> >(this->impl()->nodeTab[i]);
  }

  iterator begin() 
  {
    if (!this->impl()) return iterator(0, 0);
    else return iterator(this->impl()->nodeTab, this->impl()->nodeNr);
  }
  iterator end() { return iterator(0, 0);}

private:
  node_set(xmlNodeSet *nodes)
    : detail::wrapper<xmlNodeSet*>(xmlXPathNodeSetMerge(0, nodes)) {}
};

template <typename S>
class node_set<S>::iterator
{
public:
  typedef node_ptr<node<S> > value_type;
  typedef value_type &reference;
  typedef value_type *pointer;

  iterator(xmlNode **array, int rest)
    : current_(array),
      rest_(rest ? --rest : 0),
      tmp_(detail::ptr_factory<node<S> >(current_ ? *current_ : 0))
  {
  }
  bool operator == (iterator i) { return current_ == i.current_;}
  bool operator != (iterator i) { return !operator==(i);}
  reference operator *() { return tmp_;}
  pointer operator ->() { return &(operator *());}
  iterator operator ++(int) { increment(); return *this;}
  iterator operator ++() { iterator tmp = *this; increment(); return tmp;}
private:
  void increment() 
  {
    if (rest_)
    {
      --rest_;
      ++current_;
      tmp_ = detail::ptr_factory<node<S> >(*current_);
    }
    else
    {
      current_ = 0;
      tmp_ = value_type();
    }
  }
  xmlNode  **current_;
  int        rest_;
  value_type tmp_;
};

} // namespace boost::xml::dom
} // namespace boost::xml
} // namespace boost

#endif
