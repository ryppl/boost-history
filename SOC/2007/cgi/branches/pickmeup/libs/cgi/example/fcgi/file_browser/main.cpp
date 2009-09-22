//                    -- main.hpp --
//
//           Copyright (c) Darren Garvey 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
////////////////////////////////////////////////////////////////
//
//[fcgi_file_browser
//
// This example is a simple browser-based file browser.
//
///////////////////////////////////////////////////////////
#include <fstream>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string/find.hpp>
///////////////////////////////////////////////////////////
#include "boost/cgi/fcgi.hpp"

using std::cerr;
using std::endl;
using std::ifstream;
using namespace boost::fcgi;
namespace fs = boost::filesystem;
namespace algo = boost::algorithm;


std::size_t process_id()
{
#if defined(BOOST_WINDOWS)
  return _getpid();
#else
  return getpid();
#endif
}

/// Get the MIME type of the file. 
/**
 * @returns The MIME type as a string. Returns an empty string if the
 *          file type isn't recognised / supported.
 */
std::string get_mime_type(fs::path const& file)
{
  std::string filetype (file.filename());
  // Note: we want the string after the '.'
  std::size_t pos (filetype.rfind(".")+1);
  if (pos == std::string::npos)
    return "";
  
  filetype = filetype.substr(pos);
  algo::to_lower(filetype);
  
  /// Ordinary text files.
  if (filetype == "ini" || filetype == "txt" || filetype == "conf")
    return "text/plain";  
  else
  if (filetype == "js")
    return "application/javascript";
  else
  if (filetype == "json")
    return "application/json";
  else
  if (filetype == "css")
    return "text/css";
  else
  /// Structured text files.
  if (filetype == "html" || filetype == "htm")
    return "text/html";
  else
  if (filetype == "xml")
    return "text/xml";
  else
  if (filetype == "csv")
    return "text/csv";
  else
  if (filetype == "rtf")
    return "text/rtf";
  else
  /// Image files.
  if (filetype == "jpg" || filetype == "jpeg")
    return "image/jpeg";
  else
  if (filetype == "gif")
    return "image/gif";
  else
  if (filetype == "bmp")
    return "image/x-ms-bmp";
  else
  if (filetype == "png")
    return "image/png";
  else  
  if (filetype == "tiff")
    return "image/tiff";
  else  
  /// Audio files.
  if (filetype == "ogg")
    return "audio/ogg";
  else
  if (filetype == "mp3")
    return "audio/mpeg";
  else
  /// Video files.
  if (filetype == "avi")
    return "video/x-msvideo";
  else
  if (filetype == "wmv")
    return "video/x-ms-wmv";
  else
  /// Rich media files.
  if (filetype == "pdf")
    return "application/pdf";
  else
  if (filetype == "doc")
    return "application/msword";
  else
  if (filetype == "swf")
    return "application/x-shockwave-flash";
  else
  if (filetype == "xls")
    return "application/vnd.ms-excel";
  /// Compressed files.
  else
  if (filetype == "zip")
    return "application/zip";
  else
  if (filetype == "tar")
    return "application/x-tar";
  /// Other files.
  else
  if (filetype == "pl")
    return "application/x-perl";
  else
  if (filetype == "py")
    return "application/x-python";
  else
  if (filetype == "exe" || filetype == "dll" || filetype == "sys" ||
      filetype == "chm" || filetype == "lib" || filetype == "pdb" ||
      filetype == "obj" || filetype == "dep" || filetype == "idb" ||
      filetype == "pyd" || filetype == "sqm" || filetype == "idb" ||
      filetype == "asm" || filetype == "suo" || filetype == "sbr")
    return ""; // Not allowed to download these file types.

  return "text/plain";
}

template<typename Response, typename Client>
void show_file(Response& resp, Client& client, fs::path const& file)
{
  if (!fs::exists(file))
    resp<< content_type("text/plain")
        << "File not found.";
  else
  {
    boost::uintmax_t size (fs::file_size(file));
    cerr<< "size: " << size << endl;
    if (size > 200000000L) // Files must be < 200MB
      resp<< "File too large.";
    else
    {
      /// Check the file type is allowed.
      std::string mime_type (get_mime_type(file));
      if (!mime_type.empty())
      {
        cerr<< "MIME-type: " << mime_type << '\n';
        cerr<< "File size: " << size << '\n';
        std::string ctype (content_type(mime_type).content + "\r\n\r\n");
        write(client, boost::asio::buffer(clen));
        write(client, boost::asio::buffer(ctype));
        /// Open the file and read it as binary data.
        ifstream ifs (file.string().c_str(), std::ios::binary);
        if (ifs.is_open())
        {
          resp<< content_type(mime_type);
          //resp.flush(client);
          boost::uintmax_t bufsize = 1000;
          boost::uintmax_t read_bytes;
          char buf[1000];
          ifs.seekg(0, std::ios::beg);
          while (!ifs.eof() && size > 0)
          {
            ifs.read(buf, size < bufsize ? size : bufsize);
            read_bytes = ifs.gcount();
            size -= read_bytes;
            cerr<< "Read " << read_bytes << " bytes from the file.\n";
            //if (resp.content_length() + read_bytes >= 65000)
            //  resp.flush(client);
            //resp.write(buf, read_bytes);
            write(client, boost::asio::buffer(buf, read_bytes));
            //resp.flush(client);
          }
          //resp.send(client);
          //cerr<< "Content-length: " << resp.content_length() << '\n';
        }
      }
      else
        resp<< content_type("text/plain")
            << "File type not allowed.";
    }
  }
}
    
template<typename Response>
void show_paths(Response& resp, fs::path const& parent, bool recursive = true)
{
  if (!fs::exists(parent))
  {
    resp<< "File does not exist\n";
    return;
  }
  
  resp<< "<ul>";
  if (fs::is_directory(parent))
  {
    resp<< parent << "\n";
    resp<< "<li class=\"directory\"><a href=\"?dir="
        << parent.string() << "\">.</a></li>\n";
    if (fs::is_directory(parent.parent_path()))
      resp<< "<li class=\"directory\"><a href=\"?dir="
          << parent.parent_path().string() << "\">..</a></li>\n";
    for (fs::directory_iterator iter(parent), end; iter != end; ++iter)
    {
      if (fs::is_directory(*iter))
      {
        resp<< "<li class=\"directory\"><a href=\"?dir="
            << iter->string() << "\">" << iter->path() << "</a></li>\n";
        if (recursive)
          show_paths(resp, iter->path(), recursive);
      }
      else
      {
        // display filename only.
        resp<< "<li class=\"file\"><a href=\"?file="
            << iter->string() << "\">" << iter->path().filename()
            << "</a>";
        //if (fs::is_regular_file(iter->status()))
        //  resp<< " [" << fs::file_size(iter->path()) << " bytes]";
        resp<< "</li>\n";
      }
    }
  }
  else
  {
    resp<< "<li class=\"file\">" << "<a href=\"?file="
            << parent.string() << "\">" << parent << "</li>\n";
  }
  resp<< "</ul>";
}

/// This function accepts and handles a single request.
template<typename Request>
int handle_request(Request& req)
{
  boost::system::error_code ec;
  
  //
  // Load in the request data so we can access it easily.
  //
  req.load(parse_all); // Read and parse STDIN (ie. POST) data.

  //
  // Construct a `response` object (makes writing/sending responses easier).
  //
  response resp;

  if (req.get.count("file"))
  {
    show_file(resp, req.client(), req.get["file"]);
    req.close(resp.status(), 0);
    return 0; //commit(req, resp);
  }
  else
  if (req.get.count("dir"))
  {
    //
    // Responses in CGI programs require at least a 'Content-type' header.
    // The library provides helpers for several common headers:
    //
    resp<< content_type("text/html");
    
    // You can also stream text to a response. 
    // All of this just prints out the form 
    resp<< "<html>"
           "<head><title>FastCGI File Browser Example</title><head>"
           "<body>";

    show_paths(resp, req.get["dir"], req.get["recurse"] == "1");

    resp<< "</body></html>";
  }
  else
    resp<< content_type("text/plain")
        << "No path specified.\n";

  //resp<< header("FastCGI-client", "fcgi_file_browser");
  return commit(req, resp);
}

int main()
{
try {

  // Make a `service` (more about this in other examples).
  service s;
  
  using boost::asio::ip::tcp;

  // Accept requests on port 8001. You should configure your HTTP
  // server to try to connect on this port.
  acceptor a(s, 8001);    

  int ret(0);
  for (;;)
  {
    request req(s);

    for (;;)
    {
      a.accept(req);
      if (0 != handle_request(req))
        break;
      req.clear();
    }
  }
  
  return ret;

}catch(boost::system::system_error const& se){
  // This is the type of error thrown by the library.
  cerr<< "[fcgi] System error: " << se.what() << endl;
  return -1;
}catch(std::exception const& e){
  // Catch any other exceptions
  cerr<< "[fcgi] Exception: " << e.what() << endl;
  return -1;
}catch(...){
  cerr<< "[fcgi] Uncaught exception!" << endl;
  return -1;
}
}
//]
