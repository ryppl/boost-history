#ifndef boost_xml_reader_parser_hpp_
#define boost_xml_reader_parser_hpp_

#include <boost/xml/reader/detail.hpp>
#include <string>
#include <memory>
#include <stdexcept>
#include <stdio.h>
#include <libxml/xmlreader.h>
#include <iostream>

namespace boost
{
namespace xml
{
namespace reader
{
template <typename S> class parser;

template <typename S> struct convert;
template <>
struct convert<std::string>
{
  static xmlChar const *in(std::string const &v) { return (xmlChar *)v.c_str();}
  static std::string out(xmlChar const *v)
  { return v ? std::string((char const *)v) : std::string("");}
};

template <typename S>
class token_base
{
  friend class parser<S>;
public:
  size_t depth() const { return xmlTextReaderDepth(reader_);}
  S name() const { return convert<S>::out(xmlTextReaderConstName(reader_));}
  S value() const { return convert<S>::out(xmlTextReaderConstValue(reader_));}
  bool is_empty_element() const { return xmlTextReaderIsEmptyElement(reader_);}
  bool has_value() const { return xmlTextReaderHasValue(reader_);}
  token_type type() const { return (token_type)xmlTextReaderNodeType(reader_);}
protected:
  token_base(xmlTextReader *reader) : reader_(reader) {}

  xmlTextReader *reader_;
};

template <typename S> 
class parser
{
public:
  parser(std::string const &filename)
    : impl_(xmlReaderForFile(filename.c_str(), 0, 0)),
      status_(0)
  {
    if (!impl_) throw std::runtime_error("unable to open file");
  }
  ~parser() { xmlFreeTextReader(impl_);}
  bool next() { status_ = xmlTextReaderRead(impl_); return status_ == 1;}
  token_base<S> get_token()
  {
    token_base<S> token(impl_);
    return token;
  }
private:
  xmlTextReader *impl_;
  int status_;
};
}
}
}


#endif
