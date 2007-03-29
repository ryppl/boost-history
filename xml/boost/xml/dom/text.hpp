#ifndef boost_xml_dom_text_hpp_
#define boost_xml_dom_text_hpp_

#include <boost/xml/dom/node.hpp>

namespace boost
{
namespace xml
{
namespace dom
{

template <typename S> class element;

template <typename S>
class text : public node<S>
{
friend class element<S>;
public:
  S content() const;
  void set_content(S const &);

// private:
  text(xmlNode *a) : node<S>(a) {}
};

template <typename S>
S text<S>::content() const
{
  xmlChar *content = xmlNodeGetContent(this->impl());
  S retn = converter<S>::out(content);
  xmlFree(content);
  return retn;
}

template <typename S>
void text<S>::set_content(S const &content)
{
  xmlSetContent(this->impl(), converter<S>::in(content));
}

} // namespace boost::xml::dom
} // namespace boost::xml
} // namespace boost

#endif
