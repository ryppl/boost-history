#include <fstream>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/program_options/environment_iterator.hpp>

#include <boost/cgi/fcgi.hpp>
#include <boost/cgi/return.hpp>

using namespace std;
using namespace boost::fcgi;

// This is a file to put internal logging info into
#define LOG_FILE "/var/www/log/fcgi_echo.txt"

// This function writes the title and map contents to the ostream in an
// HTML-encoded format (to make them easier on the eye).
template<typename Map, typename OStream>
void format_map(Map& m, OStream& os, const std::string& title)
{
  os<< "<h2>" << title << "</h2>";
  if (m.empty()) os<< "NONE<br />";
  for (typename Map::const_iterator i = m.begin(), end = m.end()
      ; i != end; ++i)
  {
    os<< "<b>" << i->first << "</b> = <i>" << i->second << "</i><br />";
  }
}

template<typename Service, typename Acceptor, typename LogStream>
int handle_request(Service& s, Acceptor& a, LogStream& of)
{
  // First we construct a `request` object.
  request req(s); // This is in a protocol-specific state at the moment.

  of<< "Constructed request" << endl;
  boost::system::error_code ec;
  
  // The program blocks here until a request arrives.
  a.accept(req, ec);

  of<< "Called accept" << endl;
  // Result should be "Success".
  of<< "Accept had result: " << ec.message() << endl;

  // Load in the request data so we can access it easily.
  req.load(ec, true); // The 'true' means read and parse STDIN (ie. POST) data.

  // Construct a `response` object (makes writing/sending responses easier).
  response resp;

  // Responses in CGI programs require at least a 'Content-type' header. The
  // library provides helpers for several common headers:
  resp<< content_type("text/html")
  // You can also stream text to a response object. 
      << "Hello there, universe!<p />";

  // Use the function defined above to show some of the request data.
  format_map(req.env(), resp, "Environment Variables");
  format_map(req.GET(), resp, "GET Variables");
  format_map(req.cookie(), resp, "Cookie Variables");

  // This funky macro finishes up:
  return_(resp, req, 0);
  // It is equivalent to the below, where the third argument is represented by
  // `program_status`:
  //
  // resp.send(req.client());
  // req.close(resp.status(), program_status);
  // return program_status;
  //
  // Note: in this case `program_status == 0`.
}

int main()
{
try {

  ofstream of(LOG_FILE);
  if (!of)
  {
    std::cerr<< "Couldn't open file: \"" LOG_FILE "\"." << endl;
    return 1;
  }

  of<< boost::posix_time::second_clock::local_time() << endl;
  of<< "Going to start acceptor" << endl;

  // Make a `service` (more about this in other examples).
  service s;
  // Make an `acceptor` for accepting requests through.
  acceptor a(s);

  // This next line may become void in the future.
  a.assign(boost::asio::ip::tcp::v4(), 0);
  of<< "[a] Assigned." << endl;

  // After the initial setup, we can enter a loop to handle one request at a
  // time until there's an error of some sort.
  int ret(0);
  for (;;)
  {
    ret = handle_request(s, a, of);
    of<< "handle_requests() returned: " << ret << endl;
    if (ret)
      break;
  }
  
  return ret;

}catch(boost::system::system_error& se){
  cerr<< "[hw] System error: " << se.what() << endl;
  return 1313;
}catch(exception& e){
  cerr<< "[hw] Exception: " << e.what() << endl;
  return 666;
}catch(...){
  cerr<< "[hw] Uncaught exception!" << endl;
  return 667;
}
}
