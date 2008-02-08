#include <boost/cgi/fcgi.hpp>
#include <boost/system/system_error.hpp>
#include <fstream>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/program_options/environment_iterator.hpp>

using namespace cgi::fcgi;
using namespace std;

#define LOG_FILE "/var/www/log/fcgi_hello_world.txt"

template<typename OStream>
void accept_handler(OStream& os, const boost::system::error_code ec)
{
  os<< "Accepted connection ok!" << endl;
}


int main()
{
  std::cerr<< "[hw] Started FCGI daemon (see " LOG_FILE " for more)..." << endl;
try {
  ofstream of(LOG_FILE);
  if (!of) std::cerr<< "Couldn't open file: \"" LOG_FILE "\"." << endl;
  of<< boost::posix_time::second_clock::local_time() << endl;
  of<< "Going to start acceptor" << endl;
  service s;
  acceptor a(s);

  a.assign(boost::asio::ip::tcp::v4(), 0);
  of<< "[a] Assigned." << endl;
  bool give_up = false;

  do {

  of<< "Writing environment variables..." << endl;
  for(boost::environment_iterator i(environ), e; i != e; ++i)
  {
    of<< i->first << ", " << i->second << endl;
  }
  of<< "--------------------------------" << endl;
  request req(s);
  of<< "Constructed request" << endl;
  boost::system::error_code ec;
  a.accept(req, ec);

  of<< "Called accept" << endl;
  if (ec) { of<< "[hw] accept error: " << ec.message() << endl; return 13; }
  of<< "request.is_open() := " << req.is_open() << endl;
  of<< "request.client().is_open() := " << req.client().is_open() << endl;
  //of<< "next_layer stuff := " << req.client().connection()->next_layer().is_open() << endl;
  //a.async_accept(s, boost::bind(&accept_handler<std::ofstream>, boost::ref(of), boost::asio::placeholders::error));
  of<< "Accept had result: " << ec.message() << endl;
  boost::asio::streambuf buf;
  of<< "Start log:" << endl;

  req.load(ec, true);

  req.client().read_some(buf.prepare(64), ec);
  of<< "[a] Read some data: " << ec.message() << endl;
  //req.load(ec, true);//cgi::parse_all);
  //if (ec) { of<< "Couldn't load request: " << ec.message() << endl; return 14; }
  //for (;;)
  //{
  //  size_t br = req.client().read_some(buf.prepare(64), ec);
  //  of<< "Client wrote " << br << " bytes (max: " << std::numeric_limits<size_t>::max() << ")." << endl
  //    << string(boost::asio::buffer_cast<const char *>(buf.data()),
  //              boost::asio::buffer_size(buf.data())) << endl;
  //  if (ec) { of<< "read error: " << ec.message() << endl; return 5; }
  //}
  sleep(4);

  }while(!give_up);
  s.run();
  return 0;
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
