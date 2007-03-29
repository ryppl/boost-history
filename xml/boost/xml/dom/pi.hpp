#ifndef boost_xml_dom_pi_hpp_
#define boost_xml_dom_pi_hpp_

#include <boost/xml/dom/node.hpp>

namespace boost
{
namespace xml
{
namespace dom
{

template <typename S> class element;

template <typename S>
class pi : public node<S>
{
friend class element<S>;
public:
  S content() const;
  void set_content(S const &);

// private:
  pi(xmlNode *a) : node<S>(a) {}
};

template <typename S>
S pi<S>::content() const
{
  xmlChar *content = xmlNodeGetContent(this->impl());
  S retn = converter<S>::out(content);
  xmlFree(content);
  return retn;
}

template <typename S>
void pi<S>::set_content(S const &content)
{
  xmlSetContent(this->impl(), converter<S>::in(content));
}

} // namespace boost::xml::dom
} // namespace boost::xml
} // namespace boost

#endif
