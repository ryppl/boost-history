#ifndef boost_xml_reader_detail_hpp_
#define boost_xml_reader_detail_hpp_

#include <libxml/xmlreader.h>
#include <memory>

namespace boost
{
namespace xml
{
namespace reader
{
enum token_type
{
  none = XML_READER_TYPE_NONE,
  element = XML_READER_TYPE_ELEMENT,
  attribute = XML_READER_TYPE_ATTRIBUTE,
  text = XML_READER_TYPE_TEXT,
  cdata = XML_READER_TYPE_CDATA,
  entity_referernce = XML_READER_TYPE_ENTITY_REFERENCE,
  type_entity = XML_READER_TYPE_ENTITY,
  processing_instruction = XML_READER_TYPE_PROCESSING_INSTRUCTION,
  comment = XML_READER_TYPE_COMMENT,
  document = XML_READER_TYPE_DOCUMENT,
  document_type = XML_READER_TYPE_DOCUMENT_TYPE,
  document_fragment = XML_READER_TYPE_DOCUMENT_FRAGMENT,
  notation = XML_READER_TYPE_NOTATION,
  whitespace = XML_READER_TYPE_WHITESPACE,
  significant_whitespace = XML_READER_TYPE_SIGNIFICANT_WHITESPACE,
  end_element = XML_READER_TYPE_END_ELEMENT,
  end_entity = XML_READER_TYPE_END_ENTITY,
  xml_declaration = XML_READER_TYPE_XML_DECLARATION
};


namespace detail
{



} // namespace boost::xml::dom::detail
} // namespace boost::xml::dom
} // namespace boost::xml
} // namespace boost

#endif
