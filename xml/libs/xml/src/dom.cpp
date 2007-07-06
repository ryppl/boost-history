#include <boost/xml/dom/parser.hpp>
#include <boost/xml/dom/detail.hpp>

namespace boost
{
namespace xml
{
namespace dom
{
namespace detail
{

extern "C" void xml_error_callback(void *closure, xmlError *error)
{
  parser *p = reinterpret_cast<parser *>(closure);
  p->error_callback(*error);
}

char const *node_type_names[7] =
{
  "internal node",
  "element",
  "attribute",
  "text node",
  "cdata block",
  "processing instruction",
  "comment"
};

node_type node_type_table[22] = 
{
  INTERNAL,
  ELEMENT,   //XML_ELEMENT_NODE
  ATTRIBUTE, //XML_ATTRIBUTE_NODE
  TEXT,      //XML_TEXT_NODE
  CDATA,     //XML_CDATA_SECTION_NODE
  INTERNAL,  //XML_ENTITY_REF_NODE
  INTERNAL,  //XML_ENTITY_NODE
  PI,        //XML_PI_NODE
  COMMENT,   //XML_COMMENT_NODE
  INTERNAL,  //XML_DOCUMENT_NODE
  INTERNAL,  //XML_DOCUMENT_TYPE_NODE
  INTERNAL,  //XML_DOCUMENT_FRAG_NODE
  INTERNAL,  //XML_NOTATION_NODE
  INTERNAL,  //XML_HTML_DOCUMENT_NODE
  INTERNAL,  //XML_DTD_NODE
  INTERNAL,  //XML_ELEMENT_DECL
  INTERNAL,  //XML_ATTRIBUTE_DECL
  INTERNAL,  //XML_ENTITY_DECL
  INTERNAL,  //XML_NAMESPACE_DECL
  INTERNAL,  //XML_XINCLUDE_START
  INTERNAL,  //XML_XINCLUDE_END
  INTERNAL,  //XML_DOCB_DOCUMENT_NODE
};

} // namespace boost::xml::dom::detail
} // namespace boost::xml::dom
} // namespace boost::xml
} // namespace boost
