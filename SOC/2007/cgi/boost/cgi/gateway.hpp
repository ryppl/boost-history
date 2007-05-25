#ifndef CGI_GATEWAY_HPP_INCLUDE__
#define CGI_GATEWAY_HPP_INCLUDE__

#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>

namespace cgi {

/// The gateway class manages connections
/**
 * New connections are accepted through here, via a call to accept();
 * used/corrupted connections are closed via a call to stop(conn_ptr);
 * all connections are closed via a call to stop_all().
 *
 * If you want to use the gateway after a call to stop_all(), you must
 * call reset() and pass it the maximum number of connections the gateway
 * can take.
 */
template< typename CommonGatewayService >
class gateway
{
public:
  typedef CommonGatewayService                           service_type;
  typedef typename CommonGatewayService::protocol_type   protocol_type;
  typedef connection<protocol_type>                      connection_type;
  typedef boost::shared_ptr<connection_type>             conn_ptr;

  /// Constructor
  explicit gateway(service_type& service, int max_conns)
    : service_(service)
    , acceptor_(service.io_service())
    , available_slots_(max_conns)
  {
    //accept();
  }

  /// Destructor
  ~gateway()
  {
    stop_all();
  }

  /// Start an asychronous accept
  /**
   * This returns false unless the connection is already established. This is
   * true with standard CGI, for example, where the connection is simply a
   * wrapper over standard input/output.
   *
   * If there is a need to differentiate between a fail and a 'waiting' accept
   * then a tribool could be returned from here.
   */
  bool accept()
  {
    if( available_slots_ > 0 )
    {
      conn_ptr nc(connection_type::create());
      acceptor_.async_accept( nc->socket()
                            , boost::bind( &cgi::gateway::handle_accept
                                         , this, nc, boost::placeholders::error
                                         )
                            );
    }
    return false;
  }

  void handle_accept(conn_ptr conn, boost::system::error_code& error)
  {
    if( !error )
    {
      start(conn);
      //accept();
    }
  }

  /// Cleanly start the connection
  void start(conn_ptr cptr)
  {
    connections_.insert(cptr);
    --available_slots_;
    cptr->start();
  }

  /// Cleanly stop the connection
  void stop(conn_ptr cptr)
  {
    connections_.erase(cptr);
    ++available_slots_;
    cptr->stop();
  }

  /// Cleanly stop all connections
  void stop_all()
  {
    available_slots_ = 0; // make sure subsequent accept()s fail
    std::for_each( connection_.begin(), connections_.end()
                 , boost::bind(&gateway::stop, _1)
                 );
    available_slots_ = 0;
    connections_.clear();
  }

  /// Reset the gateway
  /**
   * All connections are gracefully closed and then the gateway is set up for
   * reuse.
   *
   * @param max_connections the available slots is reset to this value
   */
  void reset(int max_connections)
  {
    stop_all();
    available_slots_ = max_connections;
  }

  /// Increment the number of allowed connection slots
  void incr_slots(int slots = 1) { available_slots_ += slots; }
  /// Decrement the number of allowed connection slots
  void decr_slots(int slots = 1) { available_slots_ -= slots; }

private:
  service_type& service_;
  acceptor<service_type> acceptor_;
  int available_slots_;
  std::set<conn_ptr> connections_;
};

} // namespace cgi

#endif // CGI_GATEWAY_HPP_INCLUDE__

/*
 * Notes:
 *
 * 1. A FastCGI server doesn't have to use tcp sockets: it can use pipes
 * instead. Support for these will only come in when Boost.Asio supports them.
 *
 * 2. For now each request object could hold a pointer/reference to the
 * connection that it's associated with. For the forseable future that's going
 * to be enough. However, since the FastCGI spec doesn't say otherwise,
 * eventually it could happen that the response can be sent back via ANY open
 * connection. In that case, the request would have to query the gateway to
 * find an available connection before sending through it. For now, that's
 * unneccesary.
 */
