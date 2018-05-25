#ifndef TCP_CLIENT_H
#define TCP_CLIENT_H

#include <stdint.h>
#include <boost/asio.hpp>

#include "net/packet_handler.h"
#include "net/session_dispatcher.h"
#include "tcp/session.h"

namespace tcp {

namespace impl = boost::asio::ip;

class Client : public net::SessionDispatcher {
 public:
  /**
   * @brief Constructor
   *
   * @param io_service ASIO IO Service
   * @param target Target endpoint
   */
  Client(boost::asio::io_service& io_service);

  /**
   * @brief Destructor
   */
  virtual ~Client();

  /**
   * @brief Create new TCP session
   *
   * @param target Target endpoint
   */
  tcp::SessionPtr CreateSession(const impl::tcp::endpoint& target);

 private:
  boost::asio::io_service& io_service_;
};

} // namespace tcp

#endif  // TCP_CLIENT_H
