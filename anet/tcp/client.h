#ifndef TCP_CLIENT_H
#define TCP_CLIENT_H

#include <stdint.h>
#include <boost/asio.hpp>

#include "anet/net/session_handler.h"
#include "anet/net/session_dispatcher.h"
#include "anet/tcp/session.h"

namespace anet {
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
} // namespace anet

#endif  // TCP_CLIENT_H
