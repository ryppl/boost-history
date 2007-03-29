#ifndef boost_xml_dom_node_hpp_
#define boost_xml_dom_node_hpp_

#include <boost/xml/dom/nodefwd.hpp>
#include <boost/xml/dom/detail.hpp>
#include <stdexcept>

namespace boost
{
namespace xml
{
namespace dom
{

class invalid_cast : public std::invalid_argument
{
public:
  invalid_cast(std::string const &msg) 
    : std::invalid_argument("invalid cast from " + msg) {}
};

template <typename N>
class node_ptr
{
public:
  node_ptr() : impl_(0) {}
  node_ptr(N const &n) : impl_(n) {}
  //. downcasting copy-constructor (e.g. element_ptr -> node_ptr)
  template <typename N2>
  node_ptr(node_ptr<N2> n) : impl_(*n.get()) {}
  N &operator*() { return this->impl_;}
  N *operator->() { return &this->impl_;}
  N *get() { return &this->impl_;}
  operator bool() const { return this->impl_.impl();}

private:
  N impl_;
};

template <typename S>
class node : public detail::wrapper<xmlNode*>
{
  template <typename N> friend class detail::node_iterator;
  friend class node_ptr<node<S> >;
  friend class node_ptr<node<S> const>;
  friend class element<S>;
  friend class xpath<S>;
  friend node_ptr<node<S> > detail::ptr_factory<node<S> >(xmlNode *);
  friend node_ptr<node<S> const> detail::ptr_factory<node<S> const>(xmlNode *);
  template <typename T, typename N> friend T cast(node_ptr<N>);

public:
  bool operator== (node<S> const &n) { return impl() == impl(n);}

  node_type type() const { return detail::node_type_table[this->impl()->type];}
  //. Return the node's name.
  S name() const { return converter<S>::out(this->impl()->name);}
  //. Return the node's path within its document.
  S path() const;
  //. Return the node's active base (See XBase).
  S base() const;
  //. Return the node's active language.
  S lang() const;

  //. Return the parent node, if any.
  node_ptr<element<S> const> parent() const 
  { return detail::ptr_factory<element<S> >(this->impl()->parent);}
  node_ptr<element<S> > parent() 
  { return detail::ptr_factory<element<S> >(this->impl()->parent);}

protected:
  node(xmlNode *n) : detail::wrapper<xmlNode*>(n) {}
  node(node<S> const &n) : detail::wrapper<xmlNode*>(n) {}
  node<S> &operator=(node<S> const &n) 
  {
    detail::wrapper<xmlNode*>::operator=(n);
    return *this;
  }
};

template <typename S>
inline S node<S>::path() const
{
  xmlChar *path = xmlGetNodePath(this->impl());
  S retn = converter<S>::out(path);
  xmlFree(path);
  return retn;
}

template <typename S>
inline S node<S>::base() const
{
  xmlChar *path = xmlNodeGetBase(0, this->impl());
  S retn = converter<S>::out(path);
  xmlFree(path);
  return retn;
}

template <typename S>
inline S node<S>::lang() const
{
  xmlChar *path = xmlNodeGetLang(this->impl());
  S retn = converter<S>::out(path);
  xmlFree(path);
  return retn;
}

template <typename T, typename N> 
inline T cast(node_ptr<N> n)
{
  if (n->impl() && 
      detail::node_type_table[n->impl()->type] != detail::target<T>::type)
  {
    node_type type = detail::node_type_table[n->impl()->type];
    throw invalid_cast(detail::node_type_names[type]);
  }
  return detail::ptr_factory<typename detail::pointee<T>::type>(n->impl());
}

} // namespace boost::xml::dom
} // namespace boost::xml
} // namespace boost

#endif
