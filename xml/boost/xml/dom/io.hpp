#ifndef boost_xml_dom_write_hpp_
#define boost_xml_dom_write_hpp_

#include <boost/xml/dom/document.hpp>
#include <streambuf>
#include <iostream>
#include <libxml/xmlsave.h>

namespace boost
{
namespace xml
{
namespace dom
{
namespace detail
{
inline int stream_write_callback(void *context, const char *buffer, int len)
{
  std::streambuf *sb = reinterpret_cast<std::streambuf *>(context);
  int written = sb->sputn(buffer, len);
  return written;
}
inline int stream_close_callback(void *context) { return 0;}
}

template <typename S>
inline void write(document<S> const &document,
                  std::streambuf *sb,
                  std::string const &encoding = std::string())
  throw(std::runtime_error)
{
  char const *enc = encoding.c_str();
  if (enc[0] == '\0') enc = 0;
  xmlSaveCtxt *ctx = xmlSaveToIO(detail::stream_write_callback,
                                 detail::stream_close_callback,
                                 sb, enc,  XML_SAVE_FORMAT);
  long bytes = xmlSaveDoc(ctx, detail::impl_cast(&document));
  int status = xmlSaveClose(ctx);
  if(bytes == -1 || status == -1)
    throw std::runtime_error("Failed to stream document.");
}

template <typename S>
inline void write_to_file(document<S> const &document,
                          std::string const &filename,
                          std::string const &encoding = std::string())
  throw(std::runtime_error)
{
  int result = 0;
  if(!encoding.empty())
    result = xmlSaveFormatFileEnc(filename.c_str(),
				  detail::impl_cast(&document), encoding.c_str(), 1);
  else
    result = xmlSaveFormatFile(filename.c_str(), detail::impl_cast(&document), 1);

  if(result == -1)
    throw std::runtime_error("Failed to write document.");
}

} // namespace boost::xml::dom
} // namespace boost::xml
} // namespace boost

#endif
