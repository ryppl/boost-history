#ifndef boost_xml_dom_parser_hpp_
#define boost_xml_dom_parser_hpp_

#include <boost/xml/dom/document.hpp>
#include <libxml/parserInternals.h>
#include <string>
#include <stdexcept>
#include <memory>
#include <sstream>
#include <iostream>

namespace boost
{
namespace xml
{
namespace dom
{

//. A catch-all error type for not-yet classified errors.
class internal_error : public std::runtime_error
{
public:
  internal_error(std::string const &msg) : std::runtime_error(msg) {}
};

//. An error indicating the input not containing well-formed XML.
class parse_error : public std::invalid_argument
{
public:
  parse_error(std::string const &msg) : std::invalid_argument(msg) {}
};

class validation_error : public std::runtime_error
{
public:
  validation_error(std::string const &msg) : std::runtime_error(msg) {}
};

namespace detail { extern "C" void xml_error_callback(void *, xmlError *);}

//. A parser encapsulates parse-related parameters, such as non-fatal
//. errors encountered during the parse process. Other parameters might
//. be added, such as for catalog handling (See XML Catalog), or some
//. other form of caching, if possible.
class parser
{
  friend void detail::xml_error_callback(void *, xmlError *);
public:
  parser();
  ~parser();
  template <typename S>
  std::auto_ptr<document<S> > parse_file(std::string const &filename,
					 bool validate);
private:
  void error_callback(xmlError const &);

  std::string error_msg_;
};

inline parser::parser()
{
  xmlSetStructuredErrorFunc(this, detail::xml_error_callback);
}

inline parser::~parser()
{
  xmlSetStructuredErrorFunc(0, 0);
}

inline void parser::error_callback(xmlError const &error)
{
  std::ostringstream oss;
  oss << "error in " 
      << error.file << ':' << error.line << ':' << error.int2 << ":\n";
  oss << error.message << '\n';
  this->error_msg_ += oss.str();
}

template <typename S>
std::auto_ptr<document<S> > parser::parse_file(std::string const &filename,
					       bool validate)
{
  xmlInitParser();
  xmlParserCtxtPtr context = xmlCreateFileParserCtxt(filename.c_str());
  if (!context) throw parse_error("internal error");
  
  context->validate = validate;

  if (!context->directory)
    context->directory = xmlParserGetDirectory(filename.c_str());

  xmlParseDocument(context);
  // now analyze the result, i.e. look at potential errors we collected
  // via the error_callback

  if(!context->myDoc)
  {
    xmlFreeParserCtxt(context);
    throw parse_error("Document not well-formed.");
  }

  std::auto_ptr<document<S> > doc = detail::factory<S>(context->myDoc);

  bool well_formed = context->wellFormed;

  xmlFreeParserCtxt(context);

  if (!well_formed) throw parse_error("Document not well-formed.");
  else if (!this->error_msg_.empty()) throw validation_error(this->error_msg_);
  return doc;
}

//. Parse the given file and create a document containing the new infoset.
//. For a richer interface to the parser, use the parser class directly.
template <typename S>
std::auto_ptr<document<S> > parse_file(std::string const &filename,
				       bool validate)
{
  parser p;
  return p.parse_file<S>(filename, validate);
}

} // namespace boost::xml::dom
} // namespace boost::xml
} // namespace boost

#endif
