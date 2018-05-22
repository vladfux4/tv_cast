#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include <stdint.h>
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>

#include "net/packet_handler.h"
#include "net/session_dispatcher.h"
#include "tcp/session.h"

namespace tcp {

class Server : public net::SessionDispatcher {
 public:
  /**
   * @brief Session pointer
   */
  typedef boost::shared_ptr<tcp::Session> SessionPtr;

  /**
   * @brief Constructor
   *
   * @param port TCP connection local port
   */
  Server(const uint16_t port);

  /**
   * @brief Destructor
   */
  virtual ~Server();

  /**
   * @brief Run server
   */
  void Run();

  /**
   * @brief Handle TCP accept
   *
   * @param new_session TCP Session
   * @param error Error code
   */
  void HandleAccept(SessionPtr new_session,
                    const boost::system::error_code& error);

 private:
  /**
   * @brief Create new TCP session
   */
  void CreateSession();

  boost::asio::io_service io_service_;
  boost::asio::ip::tcp::acceptor acceptor_;
};

} // namespace tcp

#endif  // TCP_SERVER_H
