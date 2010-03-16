#ifndef boost_xml_dom_detail_hpp_
#define boost_xml_dom_detail_hpp_

#include <libxml/tree.h>
#include <memory>

namespace boost
{
namespace xml
{
namespace dom
{

enum node_type
{
  INTERNAL = 0,
  ELEMENT,
  ATTRIBUTE,
  TEXT,
  CDATA,
  PI,
  COMMENT,
};

namespace detail
{

extern char const *node_type_names[7];
//. node_type_table maps libxml2 node types to node_type.
//. Mask all node types not part of the public API.
extern node_type node_type_table[22];

//. Helper types used for cast<>()
template <typename N> struct target {};
template <typename S>
struct target<node_ptr<element<S> > > { static node_type const type = ELEMENT;};
template <typename S>
struct target<node_ptr<attribute<S> > > { static node_type const type = ATTRIBUTE;};
template <typename S>
struct target<node_ptr<text<S> > > { static node_type const type = TEXT;};
template <typename S>
struct target<node_ptr<cdata<S> > > { static node_type const type = CDATA;};
template <typename S>
struct target<node_ptr<pi<S> > > { static node_type const type = PI;};
template <typename S>
struct target<node_ptr<comment<S> > > { static node_type const type = COMMENT;};

template <typename P> struct pointee {};
template <typename N>
struct pointee<node_ptr<N> > { typedef N type;};

template <typename T> class wrapper;
template <typename T> T impl_cast(wrapper<T> *);
template <typename T> T impl_cast(wrapper<T> const *);

//. 
class accessor
{
protected:
  template <typename T>
  static T &impl(wrapper<T> &w) { return w.impl_;}
  template <typename T>
  static T impl(wrapper<T> const &w) { return w.impl_;}
};

//. wrapper helper class to encapsulate access to private
//. data among wrappers. This defines mutual access to the backend
//. data in an extensible way.
template <typename T>
class wrapper : public accessor
{
  friend class accessor;
  template <typename T1> friend T1 impl_cast(wrapper<T1> *);
  template <typename T1> friend T1 impl_cast(wrapper<T1> const *);
public:
  wrapper(T t) : impl_(t) {}

protected:
  using accessor::impl;
  T &impl() { return this->impl_;}
  T const impl() const { return this->impl_;}
  T release() 
  {
    T tmp = impl_; 
    impl_ = 0; 
    return tmp;
  }
private:
  T impl_;
};

template <typename T>
T impl_cast(wrapper<T> *w) { return w->impl();}
template <typename T>
T impl_cast(wrapper<T> const *w) { return w->impl();}

template <typename N>
node_ptr<N> ptr_factory(xmlNode *n) { return N(n);}

template <typename S>
std::auto_ptr<document<S> > factory(xmlDoc *d)
{
  return std::auto_ptr<document<S> >(new document<S>(d));}

template <typename N>
class node_iterator : public wrapper<xmlNode*>
{
public:
  typedef node_iterator<N> self;
  typedef node_ptr<N> value_type;
  typedef value_type &reference;
  typedef value_type *pointer;

  node_iterator(xmlNode *current = 0) : detail::wrapper<xmlNode*>(current) {}
  bool operator == (self i) { return impl() == impl(i);}
  bool operator != (self i) { return !operator==(i);}
  value_type operator *() { return detail::ptr_factory<N>(impl());}
  pointer operator ->() { return &(operator *());}
  self operator ++(int) { increment(); return *this;}
  self operator ++() { self tmp = *this; increment(); return tmp;}
  self operator --(int) { decrement(); return *this;}
  self operator --() { self tmp = *this; decrement(); return tmp;}

private:
  void increment() { impl() = impl()->next;}
  void decrement() { impl() = impl()->prev;}
};

} // namespace boost::xml::dom::detail
} // namespace boost::xml::dom
} // namespace boost::xml
} // namespace boost

#endif
