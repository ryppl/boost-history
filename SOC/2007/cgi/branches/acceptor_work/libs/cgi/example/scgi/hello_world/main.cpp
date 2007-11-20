#include <boost/cgi/scgi.hpp>
#include <fstream>

using namespace cgi::scgi;
using namespace std;

int main()
{
try {
  ofstream of("C:/Logs/scgi_hello_world.txt");
  if (!of) std::cerr<< "broken" << endl;
  service s;
  acceptor a(s, 9472);

  request req(s);
  boost::system::error_code ec;
  a.accept(req, ec);
  boost::asio::io_service ios;
  using boost::asio::ip::tcp;
  tcp::socket sock(ios);
  if (ec) { of<< "hw accept error: " << ec.message() << endl; return 13; }
  //cout<< "Hello there." << endl;

  boost::asio::streambuf buf;
  of<< "Start log:" << endl;

  //for (;;)
  {
    size_t br = req.client().read_some(buf.prepare(64), ec);
    of<< "Client wrote " << br << " bytes." << endl
      << string(buf.data().begin(), buf.data().end()) << endl;
    if (ec) { of<< "read error: " << ec.message() << endl; return 5; }
  }

  return 0;
}catch(exception& e){
  cerr<< "hw exception: " << e.what() << endl;
  return 666;
}catch(...){
  cerr<< "hw uncaught exception!" << endl;
  return 667;
}
}
