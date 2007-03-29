#ifndef boost_xml_dom_attribute_hpp_
#define boost_xml_dom_attribute_hpp_

#include <boost/xml/dom/node.hpp>

namespace boost
{
namespace xml
{
namespace dom
{

template <typename S> class element;

template <typename S>
class attribute : public node<S>
{
  friend node_ptr<attribute<S> > detail::ptr_factory<attribute<S> >(xmlNode *);
  friend class element<S>;
public:
  S name() const { return converter<S>::out(this->impl()->name);}
  S value() const;
  void set_value(S const &);

private:
  attribute(xmlNode *a) : node<S>(a) {}
};

template <typename S>
S attribute<S>::value() const
{
  xmlChar *value = xmlGetProp(this->impl()->parent, this->impl()->name);
  S retn = converter<S>::out(value);
  xmlFree(value);
  return retn;
}

template <typename S>
void attribute<S>::set_value(S const &value)
{
  xmlSetProp(this->impl()->parent, this->impl()->name, converter<S>::in(value));
}

} // namespace boost::xml::dom
} // namespace boost::xml
} // namespace boost

#endif
